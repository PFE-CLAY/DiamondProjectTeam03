// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomNavigationPoint.generated.h"

UCLASS()
class DIAMONDPROJECT_API ACustomNavigationPoint : public AActor
{
	GENERATED_BODY()
	
	
protected:
	
	
	
public:
	// Sets default values for this actor's properties
	ACustomNavigationPoint();
	
	UPROPERTY(EditAnywhere)
	bool bShouldWait;

	UPROPERTY(EditAnywhere, meta=(EditCondition="bShouldWait"))
	float TimeToWait;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
