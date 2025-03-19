// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "DiamondProject/TP_WeaponComponent.h"
#include "APlayerProtoWeapon.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDropped, ADiamondProjectCharacter*, PickUpCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFire, int, CurrentAmmo,FRotator,ShootRotation);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIAMONDPROJECT_API UAPlayerProtoWeapon : public UTP_WeaponComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float FireRatePerSecond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int MagazineSize;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDropped OnDropped;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnFire OnFire;

private:
	float LastFireTime = 0.f;
	int CurrentAmmo;
	
public:
	virtual void BeginPlay() override;
	
	virtual void Fire() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DetachWeapon();
};
