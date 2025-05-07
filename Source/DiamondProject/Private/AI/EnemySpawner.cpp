// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemySpawner.h"

#include "VectorTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"


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

void AEnemySpawner::SpawnMonster(){
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FTransform SpawnTransform = GetRandomTransform();
	AActor* EnemySpawned = GetWorld()->SpawnActor(EnemyToSpawn, &SpawnTransform , SpawnInfo);
	/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "Spawned");*/
	
}


// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AEnemySpawner::IsInActivationRange(float Distance, AActor* Player)
{
	if(Player==nullptr) return false;
	return FVector::Distance(GetActorLocation(), Player->GetActorLocation()) < Distance;
}

void AEnemySpawner::DebugDistance()
{
	if(bShouldShowDebug){
		DrawDebugSphere(GetWorld(), RootComponent->GetComponentLocation(), DistanceFromPlayer, 50, FColor::Orange, false, 100.f);
	}
}

