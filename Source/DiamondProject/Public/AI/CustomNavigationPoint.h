// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EPointType.h"
#include "GameFramework/Actor.h"
#include "Allied.h"
#include "CustomNavigationPoint.generated.h"

class APath;

UCLASS()
class DIAMONDPROJECT_API ACustomNavigationPoint : public AActor
{
	GENERATED_BODY()
	
	
protected:
	
private:
	UPROPERTY()
	int EffectIndex = 0;
	
public:
	// Sets default values for this actor's properties
	ACustomNavigationPoint();
	
	UPROPERTY(EditAnywhere)
	bool bShouldWait;

	UPROPERTY(EditAnywhere, meta=(EditCondition="bShouldWait"))
	float TimeToWait;

	UPROPERTY(EditAnywhere, Category="NavPoint")
	EPointType PointType;

	UPROPERTY(EditAnywhere, Category="NavPoint", meta=(EditCondition="PointType == EPointType::Choice"))
	TArray<APath*> PathArray;

	UPROPERTY(EditAnywhere, Category="NavPoint", meta=(EditCondition="PointType == EPointType::Choice"))
	int WantedPathIndex = 0;

	UPROPERTY()
	AAllied* Allied;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void PointEffect();

	
};
