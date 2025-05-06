// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class DIAMONDPROJECT_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = Wave)
	float WaveCooldown = 15.f;

	UPROPERTY(EditAnywhere, Category = Wave)
	int WaveEnemyCount = 3;

	UPROPERTY(EditAnywhere, Category = Wave)
	int MaxWaveEnemyCount = 5;

	UPROPERTY(EditAnywhere, Category = Wave)
	int IncrementalWaveEnemyCount = 1;

	UPROPERTY(EditAnywhere, Category = Wave)
	bool bShouldLoopWaves = true;

	UPROPERTY(EditAnywhere, Category = Wave)
	bool bShouldSpawnOnBeginplay = true;

	UPROPERTY(EditAnywhere, Category = Wave)
	bool bShouldIncrementWaves = true;

	UPROPERTY(EditDefaultsOnly, Category = Spawner)
	TSubclassOf<AEnemy> EnemyToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Spawner)
	UBoxComponent* VolumeBox;
private:
	UPROPERTY()
	FTimerHandle TimerHandle;
	
public:
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UFUNCTION(BlueprintCallable, Category = EnemySpawner)
	void SpawnWave();

	UFUNCTION(BlueprintCallable, Category = EnemySpawner)
	void StopSpawn();
	
	UFUNCTION()
	FTransform GetRandomTransform() const;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
