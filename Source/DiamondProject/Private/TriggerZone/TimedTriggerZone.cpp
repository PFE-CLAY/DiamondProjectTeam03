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
}

// Called every frame
void ATimedTriggerZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsTriggered) {
		TimeElapsed += DeltaTime;
		if (TimeElapsed >= TimeToTrigger) {
			OnTriggerZone();
		}
	}
}

// Called when actor overlap the trigger zone
void ATimedTriggerZone::OnOverlapTriggerZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Blue, FString::Printf(TEXT("%s will trigger in : %f"), *GetName(), TimeToTrigger));
	bIsTriggered = true;
}
