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
	int FirstNbActivations = 0;
	
	UPROPERTY()
	int SndNbActivations = 0;
	
	UPROPERTY()
	TObjectPtr<UPreplanDataWidget> PreplanData;

	UPROPERTY()
	TArray<TObjectPtr<APreplanAdvice>> PreplanAdvices;

	/*UPROPERTY()
	TArray<TObjectPtr<UPreplanLinkWidget>> OutLinks;

	UPROPERTY()
	TArray<TObjectPtr<UPreplanLinkWidget>> InLinks;*/
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsStepActive = false;

	UPROPERTY(BlueprintReadOnly)// saùe thing now
	bool bIsFirstStepVisible = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsFirstAlreadySeen = false;

	UPROPERTY(BlueprintReadOnly)// saùe thing now
	bool bIsSndStepVisible = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsSndAlreadySeen = false;

	
};
