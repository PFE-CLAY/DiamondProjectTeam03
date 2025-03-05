// Fill out your copyright notice in the Description page of Project Settings.

#include "DiamondProject/Public/TriggerZone/TriggerZone.h"

#include <chrono>

// Sets default values
ATriggerZone::ATriggerZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	ShapeComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->OnComponentBeginOverlap.AddDynamic(this, &ATriggerZone::OnOverlapTriggerZone);
}

void ATriggerZone::OnConstruction(const FTransform& Transform)
{
	
}

// Called when the game starts or when spawned
void ATriggerZone::BeginPlay()
{
	Super::BeginPlay();
}

void ATriggerZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsOnce && bIsTriggered) {
		OnUpdate(DeltaTime);
		if (TimeElapseToBeTriggered >= TimeBetweenTrigger) {
			OnResetUpdate(DeltaTime);
		}
	}
}

void ATriggerZone::OnUpdate(float DeltaTime)
{
	TimeElapseToBeTriggered += DeltaTime;
}

void ATriggerZone::OnResetUpdate(float DeltaTime)
{
	bIsTriggered = false;
	TimeElapseToBeTriggered = 0.0f;
	ShapeComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ShapeComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
}

// Called when actor overlap the trigger zone
void ATriggerZone::OnOverlapTriggerZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Trigger Zone"));
	bIsTriggered = true;
	if (bIsOnce && bIsTriggered) {
		SetActorTickEnabled(false);
	}
	ShapeComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShapeComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	OnTriggerZoneEvent();
}

void ATriggerZone::OnTriggerZoneEvent_Implementation()
{
	if (TriggerableActor != nullptr) {
		TriggerableActor->OnTriggerEvent_Implementation();
	} else {
		GEngine->AddOnScreenDebugMessage(static_cast<uint64>(GetUniqueID() * 10 + 1), 5.f, FColor::Red, FString::Printf(TEXT("No TriggerableActor linked to: %s"), *GetName()));
	}
}

