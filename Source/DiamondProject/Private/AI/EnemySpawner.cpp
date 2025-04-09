// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemySpawner.h"

#include "Field/FieldSystemNodes.h"


// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VolumeBox = CreateDefaultSubobject<UBoxComponent>("VolumeBox");
	RootComponent = VolumeBox;
	VolumeBox->InitBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this,  &AEnemySpawner::SpawnEnemy, SpawnCooldown, true);
}

void AEnemySpawner::SpawnEnemy()
{
	FActorSpawnParameters SpawnInfo;
	FTransform SpawnTransform = GetRandomTransform();
	AActor* EnemySpawned = GetWorld()->SpawnActor(EnemyToSpawn, &SpawnTransform , SpawnInfo);
	if(EnemySpawned == nullptr){
		return;
	}
	
}

FTransform AEnemySpawner::GetRandomTransform()
{
	FTransform Transform;
	float RandX = FMath::FRandRange(GetActorLocation().X - VolumeBox->Bounds.BoxExtent.X, GetActorLocation().X + VolumeBox->Bounds.BoxExtent.X);
	float RandY = FMath::FRandRange(GetActorLocation().Y - VolumeBox->Bounds.BoxExtent.Y, GetActorLocation().Y + VolumeBox->Bounds.BoxExtent.Y);
	float RandZ = FMath::FRandRange(GetActorLocation().Z - VolumeBox->Bounds.BoxExtent.Z, GetActorLocation().Z + VolumeBox->Bounds.BoxExtent.Z);
	FVector RandomLocation = FVector(RandX, RandY, RandZ);
	Transform.SetLocation(RandomLocation);
	return Transform;
}



// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

