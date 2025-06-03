// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SunFollower.generated.h"

UCLASS()
class DIAMONDPROJECT_API ASunFollower : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASunFollower();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* SunActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "SunFollower")
	FVector GetSunDirection() const;

	UPROPERTY(EditAnywhere)
	bool bShowDebugLines = true;

	UPROPERTY(EditAnywhere)
	float TraceDistance = 100000.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
