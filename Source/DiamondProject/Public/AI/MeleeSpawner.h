// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemySpawner.h"
#include "MeleeEnemy.h"
#include "MeleeSpawner.generated.h"

UCLASS()
class DIAMONDPROJECT_API AMeleeSpawner : public AEnemySpawner
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite)
	bool bIsDoorOpened;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Door;
private:
	UPROPERTY()
	FTimerHandle SpawnTimer;

	UPROPERTY()
	FTimerHandle LastEnemySpawnedTimer;
	
	UPROPERTY(EditAnywhere, Category = "Spawner Infos")
	float SpawnCooldown = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Spawner Infos")
    	float TimeToWaitCheck = 4.f;

	UPROPERTY(EditAnywhere, Category = "Spawner Infos")
	int MaxEnemyCount = 20;

	UPROPERTY(EditAnywhere)
	UBoxComponent* ActivationZone;

	
public:
	// Sets default values for this actor's properties
	AMeleeSpawner();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AMeleeEnemy*> SpawnedEnemies;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent)
	void OpenSpawnDoor();
	
	UFUNCTION(BlueprintImplementableEvent)
    void CloseSpawnDoor();

	UFUNCTION()
	void DeathEnemy(const AActor* DamageDealer, AActor* Enemy);

	UFUNCTION(BlueprintCallable)
	void RestartLastEnemyTimer();
private:
	virtual void Spawn() override;

	UFUNCTION()
	void WaitForAnotherSpawn();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
