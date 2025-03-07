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
#include "DiamondProject/TP_PickUpComponent.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "LoopSystem/AC_Health.h"

class UTP_PickUpComponent;

void UAPlayerProtoWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MagazineSize;

	if (TextComponent) {
		TextComponent->SetText(FText::FromString(FString::FromInt(CurrentAmmo)));;
	}
}

void UAPlayerProtoWeapon::Fire() {
	if (Character == nullptr || Character->GetController() == nullptr) {
		return;
	}

	if (CurrentAmmo <= 0) {
		return;
	}

	// Try and fire a projectile
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastFireTime < 1.0f / FireRatePerSecond) return;
	LastFireTime = CurrentTime;
	
	CurrentAmmo--;
	if (TextComponent) {
		TextComponent->SetText(FText::FromString(FString::FromInt(CurrentAmmo)));;
	}
	
	UWorld* const World = GetWorld();
		
	if (World != nullptr) {
		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

		//cast a linetrace forward to check if the projectile will hit something
		FHitResult Hit;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(Character);
		FVector End = SpawnLocation + (SpawnRotation.Vector() * 10000);
		bool bHasHit = World->LineTraceSingleByChannel(Hit,SpawnLocation, End, ECC_Visibility, CollisionParams);
		DrawDebugLine(World, SpawnLocation, End, bHasHit? FColor::Red : FColor::Green, false, 0.3f, 0, 10.f);

		if (bHasHit) {
			//Try to apply damage to the hit actor using AC_Health
			if (UAC_Health* HealthComponent = Hit.GetActor()->FindComponentByClass<UAC_Health>()) {
                HealthComponent->DecreaseHealth(Damage);
            }
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr) {
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr) {
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr) {
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
	
	if (CurrentAmmo == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Detach weapon"));
		OnDropped.Broadcast(Character);
	}
}

void UAPlayerProtoWeapon::DetachWeapon() {
	FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
	DetachFromComponent(DetachmentRules);

	Character->RemoveInstanceComponent(this);

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {
			EnhancedInputComponent->RemoveActionEventBinding(BindingIndex);
		}
	}

	Character->CurrentWeapon = nullptr;
	Character = nullptr;
}