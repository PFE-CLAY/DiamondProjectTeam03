// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DiamondProject/TP_WeaponComponent.h"
#include "APlayerProtoWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDropped, ADiamondProjectCharacter*, PickUpCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFire, int, CurrentAmmo,FRotator,ShootRotation);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIAMONDPROJECT_API UAPlayerProtoWeapon : public UTP_WeaponComponent
{
	GENERATED_BODY()

public: 

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDropped OnDropped;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnFire OnFire;

private: 
	float LastFireTime = 0.f;
	int CurrentAmmo;
	
#pragma region GameplayVariables
	UPROPERTY(EditAnywhere, Category = GameplayVariables)
	float Damage;

	UPROPERTY(EditAnywhere, Category = GameplayVariables)
	float FireRatePerSecond;

	UPROPERTY(EditAnywhere, Category = GameplayVariables)
	int MagazineSize;
#pragma endregion

#pragma region WeaponDecalVariables
	UPROPERTY(EditDefaultsOnly, Category = WeaponDecal)
	UMaterialInterface* DecalMaterial;

	UPROPERTY(EditDefaultsOnly, Category = WeaponDecal)
	float DecalSize;

	UPROPERTY(EditDefaultsOnly, Category = WeaponDecal)
	float DecalLifeSpan;
#pragma endregion

public: 
	virtual void BeginPlay() override;
	virtual void Fire() override;
 
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DetachWeapon();

public:
	UFUNCTION(BlueprintPure, Category = "Weapon")
	int GetMagazineSize() const { return MagazineSize; }

private: 
	bool IsFirePossible() const;
	void DecreaseAmmo();
	void PerformShot();
	void ProcessHit(const FHitResult& Hit, UWorld* World);
	void PlayFireEffects();
};