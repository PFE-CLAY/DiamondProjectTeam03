// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PreplanAdvice.generated.h"

UCLASS()
class DIAMONDPROJECT_API APreplanAdvice : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PreplanAdvice")
	FString PreplanID;
};
