// Fill out your copyright notice in the Description page of Project Settings.


#include "DiamondProject/Public/LDIngredients/BreakableMesh.h"


// Sets default values
ABreakableMesh::ABreakableMesh()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABreakableMesh::BreakMethod(FVector HitLocation)
{
}

// Called when the game starts or when spawned
void ABreakableMesh::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABreakableMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

