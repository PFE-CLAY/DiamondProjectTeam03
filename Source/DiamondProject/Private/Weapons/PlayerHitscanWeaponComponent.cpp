// Fill out your copyright notice in the Description page of Project Settings.

#include "DiamondProject/Public/Weapons/PlayerHitscanWeaponComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DiamondProject/UWeaponComponent.h"
#include "DiamondProject/DiamondProjectCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

void UPlayerHitscanWeaponComponent::PerformShot() const
{
    UWorld* const World = GetWorld();
    if (!World)
        return;

    APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
    const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
    const FVector SpawnLocation = PlayerController->PlayerCameraManager->GetCameraLocation();

    FHitResult Hit;
    FVector EndLocation; 

    if (Hit.GetActor() != nullptr){
        EndLocation = Hit.ImpactPoint;
    }
    else{
        EndLocation = SpawnLocation + (SpawnRotation.Vector() * 10000);
    }

    FCollisionQueryParams collisionParams;
    collisionParams.AddIgnoredActor(Character);

    bool BHasHit = World->LineTraceSingleByChannel(Hit, SpawnLocation, EndLocation, ECC_Visibility, collisionParams);
    
    if (BHasHit){
        ProcessHit(Hit, World);
        OnHit.Broadcast(Hit);
    }

    OnFire.Broadcast(CurrentAmmo, SpawnLocation + (SpawnRotation.Vector() * 1000));
}

void UPlayerHitscanWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentAmmo = AmmoOnSpawn;
}

bool UPlayerHitscanWeaponComponent::AttachWeapon(ADiamondProjectCharacter* TargetCharacter)
{
    if (!TargetCharacter)
        return false;
    
    if (TargetCharacter->CurrentWeapon){
        if (UPlayerHitscanWeaponComponent* ExistingWeapon = Cast<UPlayerHitscanWeaponComponent>(TargetCharacter->CurrentWeapon)){
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
            BindingIndex = EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UPlayerHitscanWeaponComponent::Fire).GetHandle();
        }
    }

    Character->CurrentWeapon = this;
    OnPickedUpWeapon.Broadcast();
    return true;
}
