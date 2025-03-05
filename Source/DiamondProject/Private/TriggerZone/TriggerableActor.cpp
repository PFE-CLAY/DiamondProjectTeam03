// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerZone/TriggerableActor.h"

// Sets default values
ATriggerableActor::ATriggerableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATriggerableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATriggerableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATriggerableActor::OnTriggerEvent_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnTrigger appelé dans C++ !"));
	GEngine->AddOnScreenDebugMessage(static_cast<uint64>(GetUniqueID() * 10 + 2), 5.f, FColor::Green, FString::Printf(TEXT("%s have been notify to be trigger"), *GetName()));
	OnTriggerEvent();
}
