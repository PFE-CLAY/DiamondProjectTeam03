// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerProtoWeapon.h"

#include "TP_WeaponComponent.h"
#include "DiamondProjectCharacter.h"
#include "DiamondProjectProjectile.h"
#include "KismetTraceUtils.h"
#include "PhysicsAssetRenderUtils.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

void UPlayerProtoWeapon::Fire() {
	if (Character == nullptr || Character->GetController() == nullptr) {
		return;
	}

	// Try and fire a projectile
    // Execute shot only if firerate is ready
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastFireTime < 1.0f / FireRatePerSecond) return;
	LastFireTime = CurrentTime;
	
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
		bool bHasHit = World->LineTraceSingleByChannel(Hit, SpawnLocation, End, ECollisionChannel::ECC_Visibility, CollisionParams);
		DrawDebugLine(World, SpawnLocation, End, FColor::Red, true, 1.f, 0, 10.f);
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
}

bool UPlayerProtoWeapon::AttachWeapon(ADiamondProjectCharacter* TargetCharacter) {
	return Super::AttachWeapon(TargetCharacter);
}


