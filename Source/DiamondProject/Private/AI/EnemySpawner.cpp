// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemySpawner.h"

#include "AI/MosquitoEnemy.h"
#include "Field/FieldSystemNodes.h"
#include "Kismet/GameplayStatics.h"


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

	if (!bShouldSpawnOnBeginplay) return;
	
	GetWorldTimerManager().SetTimer(TimerHandle, this,  &AEnemySpawner::SpawnWave, WaveCooldown, bShouldLoopWaves);
	
}

void AEnemySpawner::SpawnWave()
{
	if (SpawnedEnemies.Num() >= MaxEnemyCount) return;
	
	if (SpawnedEnemies.Num() + WaveEnemyCount > MaxEnemyCount){
		WaveEnemyCount = MaxEnemyCount - SpawnedEnemies.Num();
	}
	
	for (int i = 0; i < WaveEnemyCount; i++){
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FTransform SpawnTransform = GetRandomTransform();
		AActor* EnemySpawned = GetWorld()->SpawnActor(EnemyToSpawn, &SpawnTransform , SpawnInfo);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "Spawned");
		if(EnemySpawned == nullptr){
			return;
		}
		SpawnedEnemies.Add(EnemySpawned);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%d"), SpawnedEnemies.Num()));
	}

	if (!bShouldIncrementWaves) return;
	
	if(WaveEnemyCount < MaxWaveEnemyCount){
		WaveEnemyCount += IncrementalWaveEnemyCount;
	}
}

void AEnemySpawner::StopSpawn()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
	TArray<AActor*> ExistingMosquitos;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMosquitoEnemy::StaticClass(), ExistingMosquitos);
	for (auto ExistingMosquito : ExistingMosquitos)
	{
		ExistingMosquito->Destroy();
	}
}

FTransform AEnemySpawner::GetRandomTransform() const
{
	FTransform Transform;
	const float RandX = FMath::FRandRange(GetActorLocation().X - VolumeBox->Bounds.BoxExtent.X, GetActorLocation().X + VolumeBox->Bounds.BoxExtent.X);
	const float RandY = FMath::FRandRange(GetActorLocation().Y - VolumeBox->Bounds.BoxExtent.Y, GetActorLocation().Y + VolumeBox->Bounds.BoxExtent.Y);
	const float RandZ = FMath::FRandRange(GetActorLocation().Z - VolumeBox->Bounds.BoxExtent.Z, GetActorLocation().Z + VolumeBox->Bounds.BoxExtent.Z);
	const FVector RandomLocation = FVector(RandX, RandY, RandZ);
	Transform.SetLocation(RandomLocation);	
	return Transform;
}



// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

