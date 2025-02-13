// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DiamondProject/TP_WeaponComponent.h"
#include "PlayerProtoWeapon.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIAMONDPROJECT_API UPlayerProtoWeapon : public UTP_WeaponComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float FireRatePerSecond = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float MagazineSize = 30.f;

private:
	float LastFireTime = 0.f;
	
public:
	virtual void Fire() override;

	virtual bool AttachWeapon(ADiamondProjectCharacter* TargetCharacter) override;
};
