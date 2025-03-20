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
    CurrentAmmo = MagazineSize;
}

void UAPlayerProtoWeapon::Fire()
{
    if (!IsFirePossible())
        return;

    DecreaseAmmo();
    PerformShot();
    PlayFireEffects();
    
    if (CurrentAmmo == 0){
        UE_LOG(LogTemp, Warning, TEXT("Detach weapon"));
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

    if (bHasHit)
    {
        ProcessHit(hit, world);
    }

    OnFire.Broadcast(CurrentAmmo, spawnRotation);
}

void UAPlayerProtoWeapon::ProcessHit(const FHitResult& hit, UWorld* world)
{
    if (UAC_Health* healthComponent = hit.GetActor()->FindComponentByClass<UAC_Health>())
    {
        healthComponent->DecreaseHealth(Damage);
    }
    else if (DecalMaterial)
    {
        UGameplayStatics::SpawnDecalAtLocation(
            world, 
            DecalMaterial,
            FVector(DecalSize, DecalSize, DecalSize),
            hit.ImpactPoint, 
            hit.ImpactNormal.Rotation(), 
            DecalLifeSpan
        );
    }
}

void UAPlayerProtoWeapon::PlayFireEffects()
{
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
    }

    if (FireAnimation && Character)
    {
        UAnimInstance* animInstance = Character->GetMesh1P()->GetAnimInstance();
        if (animInstance)
        {
            animInstance->Montage_Play(FireAnimation, 1.f);
        }
    }
}

void UAPlayerProtoWeapon::DetachWeapon()
{
    FDetachmentTransformRules detachmentRules(EDetachmentRule::KeepWorld, false);
    DetachFromComponent(detachmentRules);

    Character->RemoveInstanceComponent(this);

    if (APlayerController* playerController = Cast<APlayerController>(Character->GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
        {
            subsystem->RemoveMappingContext(FireMappingContext);
        }
        
        if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(playerController->InputComponent))
        {
            enhancedInputComponent->RemoveActionEventBinding(BindingIndex);
        }
    }

    Character->CurrentWeapon = nullptr;
    Character = nullptr;
}