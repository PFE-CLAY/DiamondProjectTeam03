// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/PlayerHitWeaponComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DiamondProject/DiamondProjectCharacter.h"
#include "DiamondProject/DiamondProjectProjectile.h"
#include "Kismet/GameplayStatics.h"

void UPlayerHitWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = AmmoOnSpawn;
}

void UPlayerHitWeaponComponent::PerformShot() const
{
	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			ADiamondProjectProjectile* Projectile = World->SpawnActor<ADiamondProjectProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			Projectile->Initialize(Damage);

			OnFire.Broadcast(CurrentAmmo, SpawnLocation + (SpawnRotation.Vector() * 1000));
		}
	}
}

bool UPlayerHitWeaponComponent::AttachWeapon(ADiamondProjectCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UWeaponComponent>()) {
		UE_LOG(LogTemp, Warning, TEXT("Character %s already has a weapon"), *Character->GetName());
		return false;
	}

	Character->CurrentWeapon = this;

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	
	// Get current rotation and adjust roll by -15 degrees
	FRotator CurrentRotation = GetRelativeRotation();
	FRotator AdjustedRotation = GetRelativeRotation() + FRotator(0, -15, -7);
	SetRelativeRotation(AdjustedRotation);
	
	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	if (TObjectPtr<USceneComponent> PickUpComponent = this->GetChildComponent(0)) {
		PickUpComponent->DestroyComponent();
		UE_LOG(LogTemp, Warning, TEXT("Destroyed PickUpComponent"));
	}
	
	// Set up action bindings
	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UWeaponComponent::Fire);
			BindingIndex = EnhancedInputComponent->GetActionEventBindings().Num() - 1;
		}
	}

	return true;
}