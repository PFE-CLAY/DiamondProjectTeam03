// Fill out your copyright notice in the Description page of Project Settings.

#include "DiamondProject/Public/TriggerZone/TimedTriggerZone.h"

// Sets default values
ATimedTriggerZone::ATimedTriggerZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATimedTriggerZone::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenTrigger += TimeToTrigger;
}

// Called every frame
void ATimedTriggerZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATimedTriggerZone::OnUpdate(float DeltaTime)
{
	Super::OnUpdate(DeltaTime);
	TimeElapsed += DeltaTime;
}

void ATimedTriggerZone::OnResetUpdate(float DeltaTime)
{
	Super::OnResetUpdate(DeltaTime);
	OnTriggerZoneEvent();
	TimeElapsed = 0.0f;
}

// Called when actor overlap the trigger zone
void ATimedTriggerZone::OnOverlapTriggerZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(static_cast<uint64>(GetUniqueID() * 10 + 3), 5.f, FColor::Blue, FString::Printf(TEXT("%s will trigger in : %f"), *GetName(), TimeToTrigger));
	bIsTriggered = true;
}
