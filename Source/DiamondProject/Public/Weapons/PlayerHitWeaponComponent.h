// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DiamondProject/UWeaponComponent.h"
#include "PlayerHitWeaponComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class DIAMONDPROJECT_API UPlayerHitWeaponComponent : public UWeaponComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ADiamondProjectProjectile> ProjectileClass;
	
	virtual void PerformShot() const override;

	virtual bool AttachWeapon(ADiamondProjectCharacter* TargetCharacter) override;
};
