// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "EnemySpawner.generated.h"

UCLASS()
class DIAMONDPROJECT_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	AEnemy* LastSpawnedEnemy;

	UPROPERTY(EditAnywhere, Category = "Spawner Infos")
	UClass* EnemyToSpawn;

	UPROPERTY(EditAnywhere)
	USceneComponent* SpawnPoint;
	UPROPERTY()
	USceneComponent* Root;
public:
	// Sets default values for this actor's properties
	AEnemySpawner();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void Spawn(); 

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
