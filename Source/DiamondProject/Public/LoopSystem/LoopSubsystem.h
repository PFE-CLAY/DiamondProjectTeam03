// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoopSubsystem.generated.h"

class APreplanData;
class UPreplanStep;

/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API ULoopSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsInit = false;
	
public:
	UPROPERTY()
	TMap<FString, TObjectPtr<UPreplanStep>> PreplanSteps;
	
private:	
	UFUNCTION(BlueprintCallable)
	void ReloadScene();

	UFUNCTION()
	bool IsAnyPreviousStepActive(const UPreplanStep* PreplanStep);

	UFUNCTION()
	void InitializePreplanAdvices();

public:
	UFUNCTION()
	void InitializePreplan();

	UFUNCTION(BlueprintCallable)
	void ActivatePreplanStep(FString PreplanID);
};
