// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DiamondProject/UWeaponComponent.h"
#include "PlayerHitscanWeaponComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIAMONDPROJECT_API UPlayerHitscanWeaponComponent : public UWeaponComponent
{
 GENERATED_BODY()

public:
 virtual void BeginPlay() override;
 virtual void Fire() override;

 virtual bool AttachWeapon(ADiamondProjectCharacter* TargetCharacter) override;

private:
 bool IsFirePossible() const;
 void DecreaseAmmo();
 void PerformShot() const;
 void ProcessHit(const FHitResult& Hit, UWorld* World) const;
 void PlayFireEffects() const;
};