// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "CustomNavigationPoint.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "ShootingEnemy.generated.h"

class UShootPointComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyShoot);

UCLASS()
class DIAMONDPROJECT_API AShootingEnemy : public AEnemy
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Behavior")
	float BaseDetectionRange = 300;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Behavior")
	float MaxDetectionRange = 600;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Behavior")
	float DetectionRange;
	
	
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Behavior")
	UClass* Projectile;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy Behavior")
	TArray<USceneComponent*> ShootPoints;

	
	//Garbage
	
	

	UPROPERTY(BlueprintReadOnly)
	int IndexShootPoint = 0;

	UPROPERTY()
	TArray<AActor*> AllTargetActors;

	UPROPERTY()
	TArray<AActor*> AllActorsInRange;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void Shoot(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void RemoveEnemyFromSpawnerList();
	
	
public:
	
	// Sets default values for this character's properties
	AShootingEnemy();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable)
	bool IsAnyTargetInRange();
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	bool IsTargetOnSight(FRotator Rotation, FVector Location);

	UFUNCTION()
	UBehaviorTree* GetBehaviorTree() const;

	UPROPERTY(BlueprintAssignable, Category="Sunlight Detection")
	FOnEnemyShoot OnEnemyShoot;
	
	UFUNCTION()
	USceneComponent* GetNextShootPoint();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USceneComponent* GetCurrentShootPoint();

	UFUNCTION()
	void AddShootPoint(USceneComponent* ShootPoint);

	UFUNCTION()
	void RemoveShootPoint(USceneComponent* ShootPoint);
};
