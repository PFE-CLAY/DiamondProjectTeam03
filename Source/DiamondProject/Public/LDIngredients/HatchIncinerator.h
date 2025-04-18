// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HatchIncinerator.generated.h"

UCLASS()
class DIAMONDPROJECT_API AHatchIncinerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHatchIncinerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Hatch Incinerator")
	virtual void OpenHatch();

	UFUNCTION(BlueprintCallable, Category="Hatch Incinerator")
	virtual void CloseHatch();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
