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
    CurrentAmmo = AmmoOnSpawn;
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

void UAPlayerProtoWeapon::PerformShot() const
{
    UWorld* const World = GetWorld();
    if (!World)
        return;

    APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
    const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
    const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

    FHitResult Hit;
    FVector EndLocation = SpawnLocation + (SpawnRotation.Vector() * 10000);

    FCollisionQueryParams collisionParams;
    collisionParams.AddIgnoredActor(Character);

    if (bool BHasHit = World->LineTraceSingleByChannel(Hit, SpawnLocation, EndLocation, ECC_Visibility, collisionParams)){
        ProcessHit(Hit, World);
    }

    OnFire.Broadcast(CurrentAmmo, SpawnRotation);
}

void UAPlayerProtoWeapon::ProcessHit(const FHitResult& Hit, UWorld* World) const
{
    if (Hit.GetActor() != nullptr){
        if (UAC_Health* healthComponent = Hit.GetActor()->FindComponentByClass<UAC_Health>()){
            healthComponent->DecreaseHealth(Damage);
            return;
        }
    }
    
    if (DecalMaterial){
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

void UAPlayerProtoWeapon::PlayFireEffects() const
{
    if (FireSound){
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
    }

    if (FireAnimation && Character){
        if (UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance()){
            AnimInstance->Montage_Play(FireAnimation, 1.f);
        }
    }
}

bool UAPlayerProtoWeapon::AttachWeapon(ADiamondProjectCharacter* TargetCharacter)
{
    if (!TargetCharacter)
        return false;
    
    if (TargetCharacter->CurrentWeapon){
        if (UAPlayerProtoWeapon* ExistingWeapon = Cast<UAPlayerProtoWeapon>(TargetCharacter->CurrentWeapon)){
            const int NewAmmo = FMath::Clamp(ExistingWeapon->CurrentAmmo + AmmoOnSpawn, 0, ExistingWeapon->MagazineSize);
            ExistingWeapon->CurrentAmmo = NewAmmo;
            
            ExistingWeapon->OnUpdateAmmo.Broadcast(ExistingWeapon->CurrentAmmo);
            
            if (AActor* WeaponActor = GetOwner()){
                WeaponActor->Destroy();
            }
            return false;
        }
    }
    
    Character = TargetCharacter;
    
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
    AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName("GripPoint"));
    
    if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController())){
        if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())){
            subsystem->AddMappingContext(FireMappingContext, 1);
        }

        if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)){
            BindingIndex = EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UAPlayerProtoWeapon::Fire).GetHandle();
        }
    }

    Character->CurrentWeapon = this;
    OnUpdateAmmo.Broadcast(CurrentAmmo);
    return true;
}

void UAPlayerProtoWeapon::DetachWeapon()
{
    FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, false);
    DetachFromComponent(DetachmentRules);

    Character->RemoveInstanceComponent(this);

    if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController())){

        if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())){
            InputSubsystem->RemoveMappingContext(FireMappingContext);
        }

        if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)){
            EnhancedInputComponent->RemoveActionEventBinding(BindingIndex);
        }
    }

    Character->CurrentWeapon = nullptr;
    Character = nullptr;
}

int UAPlayerProtoWeapon::GetCurrentAmmo() const
{
    return CurrentAmmo;
}