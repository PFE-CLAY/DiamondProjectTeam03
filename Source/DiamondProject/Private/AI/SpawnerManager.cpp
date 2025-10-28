// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SpawnerManager.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ASpawnerManager::ASpawnerManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpawnerManager::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> SpawnerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMeleeSpawner::StaticClass(), SpawnerActors);
	PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

TArray<AMeleeSpawner*> ASpawnerManager::GetClosestSpawners()
{
	float SmallestDistance = -1.f;
	TArray<AMeleeSpawner*>  ClosestSpawnerActors;
	
	for (AMeleeSpawner* Spawner : SpawnersList)
	{
		float Distance = FVector::Distance(PlayerActor->GetActorLocation(), Spawner->GetActorLocation());
		if(Distance != -1.f && Distance < SmallestDistance)
		{
			SmallestDistance = Distance;
		}
	}
	return ClosestSpawnerActors;
}

// Called every frame
void ASpawnerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnerManager::Reset()
{
	Super::Reset();
}

