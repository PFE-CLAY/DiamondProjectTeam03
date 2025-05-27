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

	UPROPERTY(EditDefaultsOnly, Category = Spawner)
	TSubclassOf<AEnemy> EnemyToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Spawner)
	UBoxComponent* VolumeBox;



public:
	// Sets default values for this actor's properties
	AEnemySpawner();

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bShouldShowDebug = false;

	UPROPERTY()
	float DistanceFromPlayer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	UFUNCTION()
	FTransform GetRandomTransform() const;

public:

	UFUNCTION()
	void SpawnMonster();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsInActivationRange(float Distance, AActor* Actor);

	void DebugDistance();
};
