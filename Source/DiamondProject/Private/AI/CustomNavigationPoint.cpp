// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CustomNavigationPoint.h"


// Sets default values
ACustomNavigationPoint::ACustomNavigationPoint()
{

	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACustomNavigationPoint::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void ACustomNavigationPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACustomNavigationPoint::	PointEffect()
{
	
	switch (PointType){
	case EPointType::None:
		break;
	case EPointType::Crouch:
		Allied->OnCrouch();
		break;
	case EPointType::Choice:
		Allied->GetNewPath(PathArray[WantedPathIndex]);
		break;
	}
	
}


