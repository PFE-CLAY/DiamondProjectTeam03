// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemySpawner.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

UCLASS()
class DIAMONDPROJECT_API AWaveManager : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY()
	AActor* PlayerCharacter;
	
public:
	
	AWaveManager();

	UPROPERTY(EditAnywhere, Category = Wave)
	float WaveCooldown = 15.f;

	UPROPERTY(EditAnywhere, Category = Wave)
	int WaveEnemyCount = 3;
	
	UPROPERTY(EditAnywhere, Category = Wave)
	int MaxWaveEnemyCount = 3;

	UPROPERTY(EditAnywhere, Category = Wave)
	int IncrementalWaveEnemyCount = 1;

	UPROPERTY(EditAnywhere, Category = Wave)
	float Distance = 500.f;
	
	UPROPERTY(EditAnywhere, Category = Wave)
	bool bShouldLoopWaves = true;

	UPROPERTY(EditAnywhere, Category = Wave)
	bool bShouldIncrementWaves = true;

	
	
	UPROPERTY()
	TArray<AEnemySpawner*> SpawnersArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void StartSpawn();

	UFUNCTION()
	void SpawnWaves();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
