// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HatchIncinerator.generated.h"

UCLASS()
class DIAMONDPROJECT_API AHatchIncinerator : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	USceneComponent* Doors;
	
	UPROPERTY(EditAnywhere)
    USceneComponent* DroneStart;

	UPROPERTY()
	FTimerHandle TimerHandle;

public:

	UPROPERTY(BlueprintReadOnly)
	FVector DroneStartLocation;

	UPROPERTY(EditAnywhere, meta=(ClampMin = 2.0f))
	float DurationBeforeClosing = 3.f;
	
	AHatchIncinerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent , BlueprintCallable)
	void OnOpenHatch();

	

	UFUNCTION(BlueprintImplementableEvent)
	void OnCloseHatch();
	
};
