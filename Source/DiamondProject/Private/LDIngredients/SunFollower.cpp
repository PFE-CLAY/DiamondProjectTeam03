// Fill out your copyright notice in the Description page of Project Settings.


#include "SunFollower.h"

#include "Components/DirectionalLightComponent.h"


// Sets default values
ASunFollower::ASunFollower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASunFollower::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ASunFollower::GetSunDirection() const
{
	if (!SunActor)
	{
		return FVector::ZeroVector;
	}

	UDirectionalLightComponent* DirectionalLight = SunActor->FindComponentByClass<UDirectionalLightComponent>();
	if (!DirectionalLight)
	{
		return FVector::ZeroVector;
	}

	// Calculate the sun direction
	const FVector SunDirection = -DirectionalLight->GetComponentRotation().Vector();

	// Define the start and end points for the trace
	const FVector Start = GetActorLocation();
	const FVector End = Start + SunDirection * TraceDistance;
	
	// Perform the line trace
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	// Draw debug line
	DrawDebugLine(
		GetWorld(),
		Start,
		bHit ? Hit.ImpactPoint : End,
		bHit ? FColor::Red : FColor::Green,
		false,
		2.0f,
		0,
		2.0f
	);
	
	return End;
}

// Called every frame
void ASunFollower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

