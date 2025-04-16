// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Allied.h"
#include "AlliedAIController.generated.h"


UCLASS()
class DIAMONDPROJECT_API AAlliedAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAlliedAIController();

	UPROPERTY()
	AAllied* AlliedControlled;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
