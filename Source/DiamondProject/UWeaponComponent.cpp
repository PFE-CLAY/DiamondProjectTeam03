// Copyright Epic Games, Inc. All Rights Reserved.


#include "UWeaponComponent.h"
#include "DiamondProjectCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "LDIngredients/BreakableMesh.h"
#include "LoopSystem/AC_Health.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

void UWeaponComponent::ProcessHit(const FHitResult& Hit, UWorld* World) const
{
	if (Hit.GetActor() != nullptr){
		if (UAC_Health* healthComponent = Hit.GetActor()->FindComponentByClass<UAC_Health>()){
			healthComponent->DecreaseHealth(Damage, Cast<AActor>(this->GetOwner()));
			return;
		}
		else if (ABreakableMesh* Breakable = Cast<ABreakableMesh>(Hit.GetActor()))
        {
            Breakable->OnBreakMesh.Broadcast(Hit.ImpactPoint);
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

void UWeaponComponent::PlayFireEffects() const
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

void UWeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if (!IsFirePossible())
		return;

	DecreaseAmmo();
	PerformShot();
	PlayFireEffects();

	if (CurrentAmmo == 0){
		OnDropped.Broadcast(Character);
	}
}

bool UWeaponComponent::AttachWeapon(ADiamondProjectCharacter* TargetCharacter)
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

ADiamondProjectCharacter* UWeaponComponent::GetCharacter() const
{
	if (!Character) return nullptr;
	return Character;
}

void UWeaponComponent::DetachWeapon()
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

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
	{
	//Log this item name in console
	if (Character == nullptr)
	{
		return;
	}
}

int UWeaponComponent::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

USoundBase* UWeaponComponent::GetFireSound() const
{
	if (!FireSound) return nullptr;
	return FireSound;
}

bool UWeaponComponent::IsFirePossible() const
{
	if (!Character || !Character->GetController() || CurrentAmmo <= 0)
		return false;

	float currentTime = GetWorld()->GetTimeSeconds();
	return (currentTime - LastFireTime >= 1.0f / FireRatePerSecond);
}

void UWeaponComponent::DecreaseAmmo()
{
	CurrentAmmo--;
	LastFireTime = GetWorld()->GetTimeSeconds();
}

void UWeaponComponent::PerformShot() const
{
}
