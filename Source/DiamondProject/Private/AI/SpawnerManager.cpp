// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SpawnerManager.h"

#include <string>

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
	for (auto SpawnerActor : SpawnerActors)
	{
		AMeleeSpawner* MeleeSpawner = Cast<AMeleeSpawner>(SpawnerActor);
		MeleeSpawner->SpawnerManager = this;
		SpawnersList.Add(MeleeSpawner);
	}
	PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ClosestSpawners = GetClosestSpawners();
	FTimerHandle TimerBeforeStart;
	GetWorldTimerManager().SetTimer(TimerBeforeStart, this, &ASpawnerManager::StartSpawners, 1.f,false);

}

TArray<AMeleeSpawner*> ASpawnerManager::GetClosestSpawners()
{
	struct FSpawnerDist
	{
		AMeleeSpawner* Spawner;
		float Distance;
	};

	TArray<FSpawnerDist>  ClosestSpawnersInfos;
	ClosestSpawnersInfos.Init({nullptr, -1.f}, SpawnerToActivateCount);
	for (AMeleeSpawner* Spawner : SpawnersList)
	{
		if (!IsValid(Spawner))
		{
			continue;
		}

		float DistanceFromPlayer = FVector::Distance(PlayerActor->GetActorLocation(), Spawner->GetActorLocation());
		
		bool bHasBeenInserted = false;
		for (int i = 0; i < ClosestSpawnersInfos.Num(); ++i)
		{
			if (DistanceFromPlayer < ClosestSpawnersInfos[i].Distance || ClosestSpawnersInfos[i].Distance == -1)
			{
				ClosestSpawnersInfos.Insert({Spawner, DistanceFromPlayer}, i);
				bHasBeenInserted = true;
				break;
			}
		}
		
		if (!bHasBeenInserted)
		{
			ClosestSpawnersInfos.Add({Spawner, DistanceFromPlayer});
		}
		if (ClosestSpawnersInfos.Num() > SpawnerToActivateCount)
		{
			AMeleeSpawner* CurrentSpawner = ClosestSpawnersInfos[ClosestSpawnersInfos.Num() - 1].Spawner;
			if(CurrentSpawner != nullptr) CurrentSpawner->Activate(false);
			ClosestSpawnersInfos.RemoveAt(ClosestSpawnersInfos.Num() - 1);
			
		}
	}
	TArray<AMeleeSpawner*> Result;
	for (const FSpawnerDist& SpawnerInfo : ClosestSpawnersInfos)
	{
		Result.Add(SpawnerInfo.Spawner);
	}
	
	
	return Result;
}



void ASpawnerManager::ActivateClosestSpawners()
{
	
	bIsSpawnBlocked = false;
	for (AMeleeSpawner* ClosestSpawner : ClosestSpawners)
	{
		if(!ClosestSpawner->bIsActive)
		{
			if(!bIsSpawnBlocked) ClosestSpawner->Activate(true);
		}
	}
	
}

void ASpawnerManager::StartSpawners()
{
	GetClosestSpawners();
	ActivateClosestSpawners();
}

// Called every frame
void ASpawnerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetClosestSpawners();
	ActivateClosestSpawners();
}

void ASpawnerManager::Reset()
{
	Super::Reset();
}

void ASpawnerManager::OnDeathEnemy(AMeleeEnemy* Enemy)
{
	if(SpawnedEnemies.Contains(Enemy))
	{
		if(SpawnedEnemies.Num() == MaxEnemyCount)
		{
			bIsSpawnBlocked = true;
			GetWorldTimerManager().SetTimer(LastEnemySpawnedTimer, this, &ASpawnerManager::StartSpawners, LongTimeBeforeRespawn,false);

		}
		SpawnedEnemies.Remove(Enemy);
	}
}

void ASpawnerManager::AddNewEnemy(AMeleeEnemy* Enemy)
{
	SpawnedEnemies.Add(Enemy);
	if(SpawnedEnemies.Num() == MaxEnemyCount)
	{
		for(AMeleeSpawner* Spawner : SpawnersList)
		{
			Spawner->StopSpawnTimer();
		}
	}
}

