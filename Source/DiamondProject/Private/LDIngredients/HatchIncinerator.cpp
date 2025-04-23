// Fill out your copyright notice in the Description page of Project Settings.


#include "LDIngredients/HatchIncinerator.h"


// Sets default values
AHatchIncinerator::AHatchIncinerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Doors = CreateDefaultSubobject<USceneComponent>(TEXT("Doors"));
	Doors->SetupAttachment(RootComponent);
	DroneStart = CreateDefaultSubobject<USceneComponent>(TEXT("DroneStartPoint"));
	DroneStart->SetupAttachment(Doors);
	
}

// Called when the game starts or when spawned
void AHatchIncinerator::BeginPlay()
{
	Super::BeginPlay();
	DroneStartLocation = DroneStart->GetComponentLocation();
	
}

// Called every frame
void AHatchIncinerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AHatchIncinerator::OnOpenHatch_Implementation()
{
	
	GetWorldTimerManager().SetTimer(TimerHandle, this,  &AHatchIncinerator::OnCloseHatch, DurationBeforeClosing, false);
}





