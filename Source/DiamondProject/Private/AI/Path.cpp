// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Path.h"
#include "Components/BillboardComponent.h"
#include "AI/CustomNavigationPoint.h"
#include "Engine/World.h"

// Sets default values
APath::APath()
{
	PrimaryActorTick.bCanEverTick = true;
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root"));
}

// Called when the game starts or when spawned
void APath::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APath::SetAllied(AAllied* OwnerAllied)
{
	Allied = OwnerAllied;
	for(ACustomNavigationPoint* Point: PatrolPoints)
	{
		Point->Allied = Allied;
	}
}

void APath::AddNewPoint()
{
	
	FVector const Location = GetActorLocation();
	FActorSpawnParameters SpawnParameters;
	AActor* SpawnedActor = GetWorld()->SpawnActor(NavigationPointClass, &Location);
	Cast<ACustomNavigationPoint>(SpawnedActor)->Allied = Allied;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, true);
	SpawnedActor->AttachToActor(GetOwner(), AttachmentRules);
	PatrolPoints.Add(Cast<ACustomNavigationPoint>(SpawnedActor));
	
	
}

void APath::RemoveLastPoint()
{
	if(PatrolPoints.IsEmpty()) return;
	AActor* Element = PatrolPoints[PatrolPoints.Num()-1];
	PatrolPoints.RemoveAt(PatrolPoints.Num()-1);
	Element->Destroy();
}


