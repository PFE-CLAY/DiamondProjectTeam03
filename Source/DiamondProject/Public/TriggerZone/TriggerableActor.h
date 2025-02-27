// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerZone/ITriggerable.h"
#include "TriggerableActor.generated.h"

UCLASS()
class DIAMONDPROJECT_API ATriggerableActor : public AActor, public ITriggerable
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	ATriggerableActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom Event")
	void OnTriggerEvent();

	virtual void OnTriggerEvent_Implementation() override;
};
