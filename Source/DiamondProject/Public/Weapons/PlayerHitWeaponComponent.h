// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DiamondProject/UWeaponComponent.h"
#include "PlayerHitWeaponComponent.generated.h"

// Overheat related delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeatChanged, float, NewHeat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOverheatStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOverheatEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCoolingStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireFinished);

/**
 * Projectile weapon with overheat mechanic (no ammo consumption)
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIAMONDPROJECT_API UPlayerHitWeaponComponent : public UWeaponComponent
{
	GENERATED_BODY()

public:
	UPlayerHitWeaponComponent();

	virtual void BeginPlay() override;
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ADiamondProjectProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overheat)
	float WeaponHeat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overheat)
	float MaxHeat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overheat)
	float HeatGeneratedByShot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overheat)
	float TimeToRecoverHeat;

	UPROPERTY(BlueprintReadOnly, Category = Overheat)
	bool bIsOverheated;
	
	UPROPERTY(BlueprintAssignable, Category = Overheat)
	FOnHeatChanged OnHeatChanged;

	UPROPERTY(BlueprintAssignable, Category = Overheat)
	FOnOverheatStart OnOverheatStart;

	UPROPERTY(BlueprintAssignable, Category = Overheat)
	FOnOverheatEnd OnOverheatEnd;
	
	UPROPERTY(BlueprintAssignable, Category = Overheat)
	FOnCoolingStart OnCoolingStart;

	UPROPERTY(BlueprintAssignable, Category = Overheat)
	FOnFireFinished OnFireFinished;
	
	virtual void PerformShot() const override;
	virtual bool AttachWeapon(ADiamondProjectCharacter* TargetCharacter) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintPure, Category = Overheat)
	float GetHeatPercent() const { return MaxHeat > 0.f ? WeaponHeat / MaxHeat : 0.f; }

	UFUNCTION(BlueprintPure, Category = Overheat)
	bool IsOverheated() const { return bIsOverheated; }

	UFUNCTION(BlueprintPure, Category = Overheat)
	bool IsCooling() const { return bIsCooling; }

protected:
	UFUNCTION()
	void HandleFireAction();

	UFUNCTION()
	void StartHeatRecovery();

	UFUNCTION()
	void HeatRecoveryTick();

	UFUNCTION()
	void HandleFireReleased();

private:
	FTimerHandle HeatRecoveryDelayHandle;
	FTimerHandle HeatRecoveryTickHandle;

	bool bIsCooling = false;
	int32 ReleaseBindingIndex = -1;
};
