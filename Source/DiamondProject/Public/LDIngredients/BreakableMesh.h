// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakableMesh.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBreakMesh, FVector, HitLocation);

class UGeometryCollectionComponent;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breakable Mesh")
	int wallHealth = 3;
	
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breakable Mesh")
	TObjectPtr<UGeometryCollection> GeometryCollectionAsset;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Breakable Mesh")
	UGeometryCollection* GetGeometryCollectionAsset() const;
};
