// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemySpawner.h"
#include "MeleeEnemy.h"
#include "MeleeSpawner.generated.h"


class ASpawnerManager;
UCLASS()
class DIAMONDPROJECT_API AMeleeSpawner : public AEnemySpawner
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite)
	bool bIsDoorOpened;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Door;

	UPROPERTY(BlueprintReadWrite)
    bool bIsReady = false;
private:
	UPROPERTY()
	FTimerHandle SpawnTimer;

	UPROPERTY()
	FTimerHandle LastEnemySpawnedTimer;

	UPROPERTY()
	FTimerHandle TimerBeforeClosingDoor;
	
	UPROPERTY(EditAnywhere, Category = "Spawner Infos")
	float SpawnCooldown = 1.f;
	
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* ActivationZone;

	UPROPERTY(EditAnywhere, Category = "Spawner Infos", meta = (EditCondition = "!bIsManaged"))
	int WaveMonsterCount = 10;

	UPROPERTY()
	TArray<AMeleeEnemy*> SpawnedEnemies;
	
public:
	// Sets default values for this actor's properties
	AMeleeSpawner();

	UPROPERTY()
	bool bIsActive = false;

	UPROPERTY()
	ASpawnerManager* SpawnerManager;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<AMeleeEnemy*> SpawnedEnemiesInSpawner;

	UPROPERTY(EditDefaultsOnly)
	bool bIsManaged;

	

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
	

	UFUNCTION()
	void WaitForAnotherSpawn();
	UFUNCTION()
	void AddNewEnemy(AMeleeEnemy* Enemy);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Spawn() override;
	UFUNCTION()
	void Activate(bool bShouldGetActive);
	
	UFUNCTION()
	void StopSpawnTimer();
	UFUNCTION(BlueprintCallable)
	void RemoveEnemyFromArray(AMeleeEnemy* Enemy);
	
};
