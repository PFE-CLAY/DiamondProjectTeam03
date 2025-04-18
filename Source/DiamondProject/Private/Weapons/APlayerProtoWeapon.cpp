// Fill out your copyright notice in the Description page of Project Settings.

#include "DiamondProject/Public/Weapons/APlayerProtoWeapon.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DiamondProject/TP_WeaponComponent.h"
#include "DiamondProject/DiamondProjectCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "LoopSystem/AC_Health.h"

void UAPlayerProtoWeapon::BeginPlay()
{
    Super::BeginPlay();
    CurrentAmmo = AmmonOnSpawn;
}

void UAPlayerProtoWeapon::Fire()
{
    if (!IsFirePossible())
        return;

    DecreaseAmmo();
    PerformShot();
    PlayFireEffects();

    if (CurrentAmmo == 0){
        OnDropped.Broadcast(Character);
    }
}

bool UAPlayerProtoWeapon::IsFirePossible() const
{
    if (!Character || !Character->GetController() || CurrentAmmo <= 0)
        return false;

    float currentTime = GetWorld()->GetTimeSeconds();
    return (currentTime - LastFireTime >= 1.0f / FireRatePerSecond);
}

void UAPlayerProtoWeapon::DecreaseAmmo()
{
    CurrentAmmo--;
    LastFireTime = GetWorld()->GetTimeSeconds();
}

void UAPlayerProtoWeapon::PerformShot()
{
    UWorld* const world = GetWorld();
    if (!world)
        return;

    APlayerController* playerController = Cast<APlayerController>(Character->GetController());
    const FRotator spawnRotation = playerController->PlayerCameraManager->GetCameraRotation();
    const FVector spawnLocation = GetOwner()->GetActorLocation() + spawnRotation.RotateVector(MuzzleOffset);

    FHitResult hit;
    FVector endLocation = spawnLocation + (spawnRotation.Vector() * 10000);

    FCollisionQueryParams collisionParams;
    collisionParams.AddIgnoredActor(Character);

    bool bHasHit = world->LineTraceSingleByChannel(hit, spawnLocation, endLocation, ECC_Visibility, collisionParams);

    if (bHasHit){
        ProcessHit(hit, world);
    }

    OnFire.Broadcast(CurrentAmmo, spawnRotation);
}

void UAPlayerProtoWeapon::ProcessHit(const FHitResult& Hit, UWorld* World)
{
    // Check if Hit.GetActor() is valid before accessing it
    if (Hit.GetActor() != nullptr)
    {
        if (UAC_Health* healthComponent = Hit.GetActor()->FindComponentByClass<UAC_Health>())
        {
            healthComponent->DecreaseHealth(Damage);
            return;
        }
    }
    
    // If there's no actor hit or it has no health component
    if (DecalMaterial)
    {
        UGameplayStatics::SpawnDecalAtLocation(
            World,
            DecalMaterial,
            FVector(DecalSize, DecalSize, DecalSize),
            Hit.ImpactPoint,
            Hit.ImpactNormal.Rotation(),
            DecalLifeSpan
        );
    }
}

void UAPlayerProtoWeapon::PlayFireEffects()
{
    if (FireSound){
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
    }

    if (FireAnimation && Character){
        UAnimInstance* animInstance = Character->GetMesh1P()->GetAnimInstance();
        if (animInstance)
        {
            animInstance->Montage_Play(FireAnimation, 1.f);
        }
    }
}

bool UAPlayerProtoWeapon::AttachWeapon(ADiamondProjectCharacter* TargetCharacter)
{
    if (!TargetCharacter)
        return false;

    // Check if character already has a weapon
    if (TargetCharacter->CurrentWeapon)
    {
        UAPlayerProtoWeapon* existingWeapon = Cast<UAPlayerProtoWeapon>(TargetCharacter->CurrentWeapon);
        if (existingWeapon)
        {
            // Add our ammo to existing weapon, clamped to magazine size
            int newAmmo = FMath::Min(existingWeapon->CurrentAmmo + AmmonOnSpawn, existingWeapon->MagazineSize);
            existingWeapon->CurrentAmmo = newAmmo;
            
            // Notify UI about ammo update
            existingWeapon->OnUpdateAmmo.Broadcast(existingWeapon->CurrentAmmo);

            // Destroy this weapon's owner
            if (AActor* weaponActor = GetOwner())
            {
                weaponActor->Destroy();
            }
            return false; // Weapon was not attached (used for ammo)
        }
    }

    // If no weapon equipped, attach this one normally
    Character = TargetCharacter;

    // Attach the weapon to the character
    FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, true);
    AttachToComponent(Character->GetMesh1P(), attachmentRules, FName("GripPoint"));

    // Set up the input binding
    if (APlayerController* playerController = Cast<APlayerController>(Character->GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
        {
            subsystem->AddMappingContext(FireMappingContext, 1);
        }

        if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(playerController->InputComponent))
        {
            BindingIndex = enhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UAPlayerProtoWeapon::Fire).GetHandle();
        }
    }

    Character->CurrentWeapon = this;
    OnUpdateAmmo.Broadcast(CurrentAmmo);
    return true; // Weapon was successfully attached
}

void UAPlayerProtoWeapon::DetachWeapon()
{
    FDetachmentTransformRules detachmentRules(EDetachmentRule::KeepWorld, false);
    DetachFromComponent(detachmentRules);

    Character->RemoveInstanceComponent(this);

    if (APlayerController* playerController = Cast<APlayerController>(Character->GetController())){

        if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer())){
            subsystem->RemoveMappingContext(FireMappingContext);
        }

        if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(playerController->InputComponent)){
            enhancedInputComponent->RemoveActionEventBinding(BindingIndex);
        }
    }

    Character->CurrentWeapon = nullptr;
    Character = nullptr;
}

int UAPlayerProtoWeapon::GetCurrentAmmo() const
{
    return CurrentAmmo;
}