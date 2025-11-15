// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MeleeSpawner.h"

#include "AI/SpawnerManager.h"
#include "LoopSystem/AC_Health.h"


// Sets default values
AMeleeSpawner::AMeleeSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Door = CreateDefaultSubobject<UStaticMeshComponent>("Door");
	Door->SetupAttachment(Root);
	ActivationZone = CreateDefaultSubobject<UBoxComponent>("Activation Zone");
	ActivationZone->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AMeleeSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMeleeSpawner::DeathEnemy(const AActor* DamageDealer, AActor* Enemy)
{
	AMeleeEnemy* MeleeEnemy = Cast<AMeleeEnemy>(Enemy);
	RemoveEnemyFromArray(MeleeEnemy);
	if(bIsManaged) SpawnerManager->OnDeathEnemy(MeleeEnemy);
	else
	{
		if(SpawnedEnemies.Contains(MeleeEnemy))
		{
			SpawnedEnemies.Remove(MeleeEnemy);
		}
	}

}

void AMeleeSpawner::RestartLastEnemyTimer()
{
	//GetWorldTimerManager().ClearTimer(LastEnemySpawnedTimer);
	//GetWorldTimerManager().SetTimer(LastEnemySpawnedTimer, this,  &AMeleeSpawner::CloseSpawnDoor, TimeToWaitCheck,false);
}

void AMeleeSpawner::Spawn()
{
	if(TimerBeforeClosingDoor.IsValid()) GetWorldTimerManager().ClearTimer(TimerBeforeClosingDoor);
	if(!bIsDoorOpened)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "OpenDoor");
		OpenSpawnDoor();
	}
	Super::Spawn();
	UAC_Health* LastEnemyHealthComponent = Cast<UAC_Health>(LastSpawnedEnemy->GetComponentByClass(UAC_Health::StaticClass()));
	LastEnemyHealthComponent->OnDeathEvent.AddDynamic(this, &AMeleeSpawner::DeathEnemy);
	AMeleeEnemy* SpawnedMelee = Cast<AMeleeEnemy>(LastSpawnedEnemy);
	if(bIsManaged) SpawnerManager->AddNewEnemy(SpawnedMelee);
	else AddNewEnemy(SpawnedMelee);
	SpawnedEnemiesInSpawner.Add(SpawnedMelee);
	if(bIsManaged)
	{
		if(SpawnerManager->SpawnedEnemies.Num() < SpawnerManager->MaxEnemyCount)
		{
			WaitForAnotherSpawn();
		}
	}
	else
	{
		if(SpawnedEnemies.Num() < WaveMonsterCount)
		{
			WaitForAnotherSpawn();
		}
	}
	
}

void AMeleeSpawner::AddNewEnemy(AMeleeEnemy* Enemy)
{
	SpawnedEnemies.Add(Enemy);
	if(SpawnedEnemies.Num() == WaveMonsterCount)
	{
		StopSpawnTimer();
	}
}

void AMeleeSpawner::WaitForAnotherSpawn()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this,  &AMeleeSpawner::Spawn, SpawnCooldown, false);
}

// Called every frame
void AMeleeSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMeleeSpawner::Activate(bool bShouldGetActive)
{
	if(bShouldGetActive)
	{	
		
		if(!bIsActive && SpawnerManager->SpawnedEnemies.Num() < SpawnerManager->MaxEnemyCount && bIsReady)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, "SPAWN AU DEBUT LO");
			Spawn(); // Lance la série de spawn
			bIsActive = true;
		}
	}
	else
	{
		StopSpawnTimer();	
		bIsActive = false;

	}
}

void AMeleeSpawner::StopSpawnTimer()
{
	GetWorldTimerManager().ClearTimer(SpawnTimer);
	
	if(bIsDoorOpened) RestartLastEnemyTimer();
	
}

void AMeleeSpawner::RemoveEnemyFromArray(AMeleeEnemy* Enemy)
{
	if(SpawnedEnemiesInSpawner.Contains(Enemy))
	{
		SpawnedEnemiesInSpawner.Remove(Enemy);
		if(SpawnedEnemiesInSpawner.Num() <= 0)
		{
			if(TimerBeforeClosingDoor.IsValid()) GetWorldTimerManager().ClearTimer(TimerBeforeClosingDoor);
			GetWorldTimerManager().SetTimer(TimerBeforeClosingDoor, this, &AMeleeSpawner::CloseSpawnDoor, 1.f, false);
			bIsActive = false;
		}
	}
	
}
































