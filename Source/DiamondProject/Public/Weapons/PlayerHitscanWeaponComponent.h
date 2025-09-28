// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DiamondProject/UWeaponComponent.h"
#include "PlayerHitscanWeaponComponent.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHit, FHitResult, HitInfo);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIAMONDPROJECT_API UPlayerHitscanWeaponComponent : public UWeaponComponent
{
 GENERATED_BODY()

public:

 UPROPERTY(BlueprintAssignable, Category = "Events")
 FOnHit OnHit;
 
 virtual void BeginPlay() override;

 virtual bool AttachWeapon(ADiamondProjectCharacter* TargetCharacter) override;

private:
 virtual void PerformShot() const override;
};