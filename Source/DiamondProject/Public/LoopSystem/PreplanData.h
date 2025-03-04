// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PreplanData.generated.h"

/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API UPreplanData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PreplanData")
	FString PreplanID;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<TObjectPtr<UPreplanData>> PreviousDatas;
};
