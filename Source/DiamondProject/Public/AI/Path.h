// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "Path.generated.h"

class ACustomNavigationPoint;
class AAllied;

UCLASS()
class DIAMONDPROJECT_API APath : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	USplineComponent* PathSpline;

	UPROPERTY(EditDefaultsOnly)
	UBillboardComponent* BillboardComponent;

public:
	// Sets default values for this actor's properties
	APath();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "NavPoint")
	TArray<TObjectPtr<ACustomNavigationPoint>> PatrolPoints;

	UPROPERTY(EditDefaultsOnly, Category = "NavPoint")
	UClass* NavigationPointClass;

	UPROPERTY()
	AAllied* Allied;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UFUNCTION(CallInEditor, Category="NavPoint")
	void AddNewPoint();

	UFUNCTION(CallInEditor, Category="NavPoint")
	void RemoveLastPoint();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetAllied(AAllied* OwnerAllied);
};
