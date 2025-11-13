// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "UWeaponComponent.generated.h"

class ADiamondProjectCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDropped, ADiamondProjectCharacter*, PickUpCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFire, int, CurrentAmmo, FVector, HitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateAmmo, int, newAmmoCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickedUp);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIAMONDPROJECT_API UWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

protected:
	int CurrentAmmo;
	int32 BindingIndex;
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDropped OnDropped;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnFire OnFire;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUpdateAmmo OnUpdateAmmo;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPickedUp OnPickedUpWeapon;

#pragma region GameplayVariables
	UPROPERTY()
	float LastFireTime = 0.f;

	UPROPERTY(EditAnywhere, Category = GameplayVariables)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayVariables)
	float FireRatePerSecond;

	UPROPERTY(EditAnywhere, Category = GameplayVariables)
	int AmmoOnSpawn;

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
	
	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/** Sets default values for this component's properties */
	UWeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual bool AttachWeapon(ADiamondProjectCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	UFUNCTION(BlueprintPure, Category = Weapon)
	virtual ADiamondProjectCharacter* GetCharacter() const;

	UFUNCTION(BlueprintPure, Category = Weapon)
	int GetCurrentAmmo() const;

	UFUNCTION(BlueprintPure, Category = Weapon)
	USoundBase* GetFireSound() const;

	UFUNCTION(BlueprintPure, Category = Weapon)
	int GetAmmoOnSpawn() const { return AmmoOnSpawn; }

	UFUNCTION(BlueprintPure, Category = Weapon)
	int GetMagazineSize() const { return MagazineSize; }

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void DetachWeapon();
protected:
	/** The Character holding this weapon*/
	UPROPERTY()
	TObjectPtr<ADiamondProjectCharacter> Character;
	
	UFUNCTION()
	bool IsFirePossible() const;
	
	UFUNCTION()
	void DecreaseAmmo();

	virtual void PerformShot() const;
	void ProcessHit(const FHitResult& Hit, UWorld* World) const;
	void PlayFireEffects() const;
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
