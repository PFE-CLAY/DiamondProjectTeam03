// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemySpawner.h"
#include "AI/MeleeEnemy.h"


// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetupAttachment(RootComponent);
	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(Root);
	
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemySpawner::Spawn()
{
	const FVector SpawnPosition = SpawnPoint->GetComponentLocation();
	const FRotator SpawnRotation = FRotator(0, 0, SpawnPoint->GetComponentRotation().Yaw);
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AEnemy* NewEnemy = Cast<AEnemy>(GetWorld()->SpawnActor(EnemyToSpawn, &SpawnPosition, &SpawnRotation, SpawnParameters));
	NewEnemy->SpawnDefaultController();
	NewEnemy->AIController = NewEnemy->GetController<AAIController>();
	LastSpawnedEnemy = NewEnemy;
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

