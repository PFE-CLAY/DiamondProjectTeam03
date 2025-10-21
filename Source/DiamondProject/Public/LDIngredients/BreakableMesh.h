// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakableMesh.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBreakMesh, FVector, HitLocation);

UCLASS()
class DIAMONDPROJECT_API ABreakableMesh : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABreakableMesh();

	UPROPERTY(BlueprintAssignable)
	FOnBreakMesh OnBreakMesh;

	void BreakMethod(FVector HitLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
