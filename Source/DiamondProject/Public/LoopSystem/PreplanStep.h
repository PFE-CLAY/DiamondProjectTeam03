// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PreplanStep.generated.h"

/**
 * 
 */
class UPreplanDataWidget;
class APreplanAdvice;

UCLASS()
class DIAMONDPROJECT_API UPreplanStep : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int NbActivations = 0;
	
	UPROPERTY()
	TObjectPtr<UPreplanDataWidget> PreplanData;

	UPROPERTY()
	TArray<TObjectPtr<APreplanAdvice>> PreplanAdvices;
	
	UPROPERTY()
	bool bIsStepActive = false;

	UPROPERTY()
	bool bIsStepVisible = false;
};
