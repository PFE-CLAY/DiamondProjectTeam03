// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MeleeSpawner.h"

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
	//TODO => spawn in spawner manager instead of in start
	
}

void AMeleeSpawner::DeathEnemy(const AActor* DamageDealer, AActor* Enemy)
{
	AMeleeEnemy* MeleeEnemy = Cast<AMeleeEnemy>(Enemy);
	if(SpawnedEnemies.Contains(MeleeEnemy))
	{
		if(SpawnedEnemies.Num() == MaxEnemyCount)
		{
			//GetWorldTimerManager().SetTimer(LastEnemySpawnedTimer, this,  &AMeleeSpawner::Spawn, LongTimeBeforeRespawn,false);
		}
		SpawnedEnemies.Remove(MeleeEnemy);
	}

}

void AMeleeSpawner::RestartLastEnemyTimer()
{
	GetWorldTimerManager().ClearTimer(LastEnemySpawnedTimer);
	//LastEnemySpawnedTimer.Invalidate();
	GetWorldTimerManager().SetTimer(LastEnemySpawnedTimer, this,  &AMeleeSpawner::CloseSpawnDoor, TimeToWaitCheck,false);
}

void AMeleeSpawner::Spawn()
{
	//TODO => les 40 secondes d'attente si max Enemy est atteint mais qu'un enemy meurt
	if(!bIsDoorOpened)
	{
		OpenSpawnDoor();
		
	}
	Super::Spawn();
	UAC_Health* LastEnemyHealthComponent = Cast<UAC_Health>(LastSpawnedEnemy->GetComponentByClass(UAC_Health::StaticClass()));
	LastEnemyHealthComponent->OnDeathEvent.AddDynamic(this, &AMeleeSpawner::DeathEnemy);
	SpawnedEnemies.Add(Cast<AMeleeEnemy>(LastSpawnedEnemy));
	
	if(SpawnedEnemies.Num() < MaxEnemyCount)
	{
		WaitForAnotherSpawn();
		
	}
}



void AMeleeSpawner::WaitForAnotherSpawn()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this,  &AMeleeSpawner::Spawn, SpawnCooldown, false);
	RestartLastEnemyTimer();
}



// Called every frame
void AMeleeSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

