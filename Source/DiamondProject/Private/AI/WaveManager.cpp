// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/WaveManager.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AWaveManager::AWaveManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), OutActors);
	for (auto Actor : OutActors){
		AEnemySpawner* Spawner = Cast<AEnemySpawner>(Actor);
		if(Spawner == nullptr) continue;
		SpawnersArray.Add(Spawner);
		Spawner->DistanceFromPlayer = Distance;
		Spawner->DebugDistance();
	}

	StartSpawn();
}

void AWaveManager::StartSpawn()
{
	SpawnWaves();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this,  &AWaveManager::SpawnWaves, WaveCooldown, bShouldLoopWaves);
}

void AWaveManager::SpawnWaves()
{
	/*TArray<AEnemySpawner*>* ActiveSpawners = new TArray<AEnemySpawner*>();
	for (auto Spawner : SpawnersArray){
		if(Spawner->IsInActivationRange(Distance)) ActiveSpawners->Add(Spawner);
	}

	if(SpawnersArray.Num() <=  0) return;

	for (int i = 0; i < WaveEnemyCount; i++){
		int RandomIndex = FMath::RandRange(0, ActiveSpawners->Num() - 1);
		AEnemySpawner* RandSpawner = ActiveSpawners[0]; 
	}*/

	if(SpawnersArray.Num() <=  0) return;

	TArray<AEnemySpawner*> ActiveSpawners;
	for (auto Spawner : SpawnersArray){
		if(Spawner->IsInActivationRange(Distance, PlayerCharacter)){
			ActiveSpawners.Add(Spawner);
			
		}
	}

	if(ActiveSpawners.Num() <=  0) return;

	for (int i = 0; i < WaveEnemyCount; i++){
		int Max = ActiveSpawners.Num() - 1;
		int RandomIndex = FMath::RandRange(0, Max);
		AEnemySpawner* RandSpawner = ActiveSpawners[RandomIndex];
		RandSpawner->SpawnMonster();
		
	}

	if(WaveEnemyCount == MaxWaveEnemyCount) return;
	WaveEnemyCount += IncrementalWaveEnemyCount;
	
}

// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

