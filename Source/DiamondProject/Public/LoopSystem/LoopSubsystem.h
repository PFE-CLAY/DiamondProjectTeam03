// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoopSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API ULoopSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void ReloadScene();
};
