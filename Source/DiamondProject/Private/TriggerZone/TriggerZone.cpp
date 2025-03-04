// Fill out your copyright notice in the Description page of Project Settings.

#include "DiamondProject/Public/TriggerZone/TriggerZone.h"

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

// Called when actor overlap the trigger zone
void ATriggerZone::OnOverlapTriggerZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Trigger Zone"));
	bIsTriggered = true;
	OnTriggerZone();
}

// Called every frame
void ATriggerZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

