// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeSpawner.h"
#include "GameFramework/Actor.h"
#include "SpawnerManager.generated.h"

UCLASS()
class DIAMONDPROJECT_API ASpawnerManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnerManager();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<AMeleeEnemy*> SpawnedEnemies;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AMeleeSpawner*> ClosestSpawners;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AMeleeSpawner*> SpawnersList;

	UPROPERTY(EditAnywhere, Category = SpawnerManager)
	int SpawnerToActivateCount;

	UPROPERTY(EditAnywhere, Category = SpawnerInfos)
	int MaxEnemyCount;

	UPROPERTY(EditAnywhere, Category = SpawnerInfos)
	float LongTimeBeforeRespawn = 40.f;

	UPROPERTY()
	AActor* PlayerActor;

	UPROPERTY()
	FTimerHandle LastEnemySpawnedTimer;

	UPROPERTY()
	bool bIsSpawnBlocked = false;

	UPROPERTY()
	bool bIsActive = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	TArray<AMeleeSpawner*>  GetClosestSpawners();

private:
	
	
	
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Reset();

	UFUNCTION()
	void OnDeathEnemy(AMeleeEnemy* Enemy);

	UFUNCTION()
	void AddNewEnemy(AMeleeEnemy* Enemy);

	UFUNCTION(BlueprintCallable)
	void SetManagerActive();

	UFUNCTION()
	void StartSpawners();
};
