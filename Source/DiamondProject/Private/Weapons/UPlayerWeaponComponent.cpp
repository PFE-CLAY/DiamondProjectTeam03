// Fill out your copyright notice in the Description page of Project Settings.

#include "DiamondProject/Public/Weapons/UPlayerWeaponComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DiamondProject/UWeaponComponent.h"
#include "DiamondProject/DiamondProjectCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "LoopSystem/AC_Health.h"

void UPlayerWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentAmmo = AmmoOnSpawn;
}

void UPlayerWeaponComponent::Fire()
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

bool UPlayerWeaponComponent::IsFirePossible() const
{
    if (!Character || !Character->GetController() || CurrentAmmo <= 0)
        return false;

    float currentTime = GetWorld()->GetTimeSeconds();
    return (currentTime - LastFireTime >= 1.0f / FireRatePerSecond);
}

void UPlayerWeaponComponent::DecreaseAmmo()
{
    CurrentAmmo--;
    LastFireTime = GetWorld()->GetTimeSeconds();
}

void UPlayerWeaponComponent::PerformShot() const
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

void UPlayerWeaponComponent::ProcessHit(const FHitResult& Hit, UWorld* World) const
{
    if (Hit.GetActor() != nullptr){
        if (UAC_Health* healthComponent = Hit.GetActor()->FindComponentByClass<UAC_Health>()){
            healthComponent->DecreaseHealth(Damage, Cast<AActor>(this->GetOwner()));
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

void UPlayerWeaponComponent::PlayFireEffects() const
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

bool UPlayerWeaponComponent::AttachWeapon(ADiamondProjectCharacter* TargetCharacter)
{
    if (!TargetCharacter)
        return false;
    
    if (TargetCharacter->CurrentWeapon){
        if (UPlayerWeaponComponent* ExistingWeapon = Cast<UPlayerWeaponComponent>(TargetCharacter->CurrentWeapon)){
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
            BindingIndex = EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UPlayerWeaponComponent::Fire).GetHandle();
        }
    }

    Character->CurrentWeapon = this;
    OnPickedUpWeapon.Broadcast();
    return true;
}

void UPlayerWeaponComponent::DetachWeapon()
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

int UPlayerWeaponComponent::GetCurrentAmmo() const
{
    return CurrentAmmo;
}

ADiamondProjectCharacter* UPlayerWeaponComponent::GetCharacter() const
{
    if (!Character) return nullptr;
    return Character;
}

USoundBase* UPlayerWeaponComponent::GetFireSound() const
{
    if (!FireSound) return nullptr;
    return FireSound;
}
