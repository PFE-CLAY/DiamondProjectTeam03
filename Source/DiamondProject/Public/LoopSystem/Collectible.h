// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Collectible.generated.h"

/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API Acollectible : public AActor{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CollectibleData")
	FString CollectibleID;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="CollectibleData")
	bool bHasBeenCollected = false;
};
