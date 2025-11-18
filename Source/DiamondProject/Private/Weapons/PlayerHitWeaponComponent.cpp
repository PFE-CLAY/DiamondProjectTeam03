// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/PlayerHitWeaponComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DiamondProject/DiamondProjectCharacter.h"
#include "DiamondProject/DiamondProjectProjectile.h"
#include "TimerManager.h"

UPlayerHitWeaponComponent::UPlayerHitWeaponComponent()
{
	WeaponHeat = 0.f;
	MaxHeat = 100.f;
	HeatGeneratedByShot = 10.f;
	TimeToRecoverHeat = 3.f;
	bIsOverheated = false;
	bIsCooling = false;
	TArray<USceneComponent*> SceneComponents;
	GetOwner()->GetComponents<USceneComponent>(SceneComponents);
 
	for (USceneComponent* Comp : SceneComponents)
	{
		if (Comp)
		{
		}
	}
}

void UPlayerHitWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = INT_MAX; // Prevent from dropping weapon
}

void UPlayerHitWeaponComponent::HandleFireAction()
{
	// prevent fire when overheated or not cooled fully 
	if (bIsOverheated || !Character || !Character->GetController())
		return;

	float currentTime = GetWorld()->GetTimeSeconds();
	if (currentTime - LastFireTime < 1.0f / FireRatePerSecond)
		return;

	LastFireTime = currentTime;

	// stop cooling because player fired
	if (GetWorld()->GetTimerManager().IsTimerActive(HeatRecoveryTickHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(HeatRecoveryTickHandle);
		bIsCooling = false;
	}
	GetWorld()->GetTimerManager().ClearTimer(HeatRecoveryDelayHandle);
	
	PerformShot();
	PlayFireEffects();
	
	WeaponHeat = FMath::Clamp(WeaponHeat + HeatGeneratedByShot, 0.f, MaxHeat);
	OnHeatChanged.Broadcast(WeaponHeat);

	if (WeaponHeat >= MaxHeat - KINDA_SMALL_NUMBER)
	{
		bIsOverheated = true;
		OnOverheatStart.Broadcast();
		const float OverheatDelay = 2.0f;
		GetWorld()->GetTimerManager().SetTimer(HeatRecoveryDelayHandle, this, &UPlayerHitWeaponComponent::StartHeatRecovery, OverheatDelay, false);
	}
}

void UPlayerHitWeaponComponent::HandleFireReleased()
{
	if (!GetWorld()) return;

	OnFireFinished.Broadcast();
	if (bIsOverheated)
	{
		// start overheat delay if not started
		if (!GetWorld()->GetTimerManager().IsTimerActive(HeatRecoveryDelayHandle) && !GetWorld()->GetTimerManager().IsTimerActive(HeatRecoveryTickHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(HeatRecoveryDelayHandle, this, &UPlayerHitWeaponComponent::StartHeatRecovery, 2.0f, false);
		}
	} else
	{
		// Not overheated, start cooling
		if (WeaponHeat > 0.f && !bIsCooling)
		{
			StartHeatRecovery();
		}
	}

}

void UPlayerHitWeaponComponent::StartHeatRecovery()
{
	if (!GetWorld()) return;
	
	GetWorld()->GetTimerManager().ClearTimer(HeatRecoveryDelayHandle);

	if (WeaponHeat <= 0.f)
	{
		if (bIsOverheated)
		{
			bIsOverheated = false;
			OnOverheatEnd.Broadcast();
		}
		bIsCooling = false;
		return;
	}
	if (TimeToRecoverHeat <= 0.f)
	{
		WeaponHeat = 0.f;
		OnHeatChanged.Broadcast(WeaponHeat);
		if (bIsOverheated)
		{
			bIsOverheated = false;
			OnOverheatEnd.Broadcast();
		}
		bIsCooling = false;
		return;
	}

	if (bIsCooling) return;
	bIsCooling = true;
	OnCoolingStart.Broadcast();

	const float TickRate = 0.05f; // 20 Hz
	GetWorld()->GetTimerManager().SetTimer(HeatRecoveryTickHandle, this, &UPlayerHitWeaponComponent::HeatRecoveryTick, TickRate, true);
}

void UPlayerHitWeaponComponent::HeatRecoveryTick()
{
	if (!GetWorld()) return;
	if (WeaponHeat <= 0.f)
	{
		WeaponHeat = 0.f;
		GetWorld()->GetTimerManager().ClearTimer(HeatRecoveryTickHandle);
		if (bIsOverheated)
		{
			bIsOverheated = false;
			OnOverheatEnd.Broadcast();
		}
		if (bIsCooling) { bIsCooling = false; }
		OnHeatChanged.Broadcast(WeaponHeat);
		return;
	}

	float TotalTime = TimeToRecoverHeat;
	float TickRate = GetWorld()->GetTimerManager().GetTimerRate(HeatRecoveryTickHandle);
	float HeatReductionPerSecond = MaxHeat / TotalTime;
	float HeatReductionThisTick = HeatReductionPerSecond * TickRate;
	WeaponHeat = FMath::Clamp(WeaponHeat - HeatReductionThisTick, 0.f, MaxHeat);
	OnHeatChanged.Broadcast(WeaponHeat);
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
			if (!PlayerController) return;
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			//const FRotator SpawnRotation = GetOwner()->GetActorRotation();
			const FVector ParentLocation = GetOwner()->GetActorLocation();
			const FVector SpawnOffset = SpawnRotation.RotateVector(MuzzleOffset);
			const FVector SpawnLocation = ParentLocation; // + SpawnOffset;
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ADiamondProjectProjectile* Projectile = World->SpawnActor<ADiamondProjectProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (Projectile)
			{
				Projectile->Initialize(Damage);
			}
			OnFire.Broadcast(CurrentAmmo, SpawnLocation + (SpawnRotation.Vector() * 1000));
		}
	}
}

bool UPlayerHitWeaponComponent::AttachWeapon(ADiamondProjectCharacter* TargetCharacter)
{
	bool bAttached = Super::AttachWeapon(TargetCharacter);
	if (!bAttached) return false;

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			if (BindingIndex >= 0 && BindingIndex < EnhancedInputComponent->GetActionEventBindings().Num())
			{
				EnhancedInputComponent->RemoveActionEventBinding(BindingIndex);
			}
			// Bind press/trigger for firing and release for starting cooling
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UPlayerHitWeaponComponent::HandleFireAction);
			int32 TriggeredIndex = EnhancedInputComponent->GetActionEventBindings().Num() - 1;
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UPlayerHitWeaponComponent::HandleFireReleased);
			int32 CompletedIndex = EnhancedInputComponent->GetActionEventBindings().Num() - 1;
			BindingIndex = TriggeredIndex;
			ReleaseBindingIndex = CompletedIndex;
		}
	}

	return true;
}

void UPlayerHitWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HeatRecoveryDelayHandle);
		GetWorld()->GetTimerManager().ClearTimer(HeatRecoveryTickHandle);
	}
	
	if (Character)
	{
		if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent))
			{
				if (ReleaseBindingIndex >= 0 && ReleaseBindingIndex < EnhancedInputComponent->GetActionEventBindings().Num())
				{
					EnhancedInputComponent->RemoveActionEventBinding(ReleaseBindingIndex);
				}
				if (BindingIndex >= 0 && BindingIndex < EnhancedInputComponent->GetActionEventBindings().Num())
				{
					EnhancedInputComponent->RemoveActionEventBinding(BindingIndex);
				}
			}
		}
	}
	Super::EndPlay(EndPlayReason);
}
