﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PreplanDataActor.generated.h"

/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API APreplanDataActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PreplanData")
	FString PreplanID;

	UPROPERTY(EditAnywhere, Category="PreplanData")
	int NbActivationsRequired = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PreplanData")
	bool bIsActiveOnStart = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="PreplanData")
	TArray<TObjectPtr<APreplanDataActor>> PreviousDatas;

	UPROPERTY(EditAnywhere, Category="PreplanData")
	bool bShouldActivateDream = false;

	UPROPERTY(EditAnywhere, Category="PreplanData", meta = (EditCondition="bShouldActivateDream"))
	TObjectPtr<UDataTable> DreamSubtitles;
};
