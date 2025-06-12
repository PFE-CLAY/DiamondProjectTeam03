// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PreplanStep.generated.h"

/**
 * 
 */
class UPreplanLinkWidget;
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
	TArray<TObjectPtr<UPreplanLinkWidget>> OutLinks;

	UPROPERTY()
	TArray<TObjectPtr<UPreplanLinkWidget>> InLinks;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsStepActive = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsStepVisible = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsAlreadySeen = false;
};
