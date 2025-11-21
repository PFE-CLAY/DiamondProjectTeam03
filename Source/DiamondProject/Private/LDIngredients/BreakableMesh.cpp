// Fill out your copyright notice in the Description page of Project Settings.

#include "LDIngredients/BreakableMesh.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ABreakableMesh::ABreakableMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABreakableMesh::BeginPlay()
{
	Super::BeginPlay();

}

void ABreakableMesh::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GeometryCollection = FindComponentByClass<UGeometryCollectionComponent>();
	if (GeometryCollection && GeometryCollectionAsset)
	{
		GeometryCollection->SetRestCollection(GeometryCollectionAsset);
	}
}

// Called every frame
void ABreakableMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableMesh::BreakMethod(FVector HitLocation)
{
	if (--wallHealth <= 0)
	{
		OnBreakMesh.Broadcast(HitLocation);
	}
}

UGeometryCollection* ABreakableMesh::GetGeometryCollectionAsset() const
{
	return GeometryCollectionAsset;
}

