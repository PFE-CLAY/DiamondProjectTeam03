// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "CustomNavigationPoint.h"
#include "Components/BoxComponent.h"
#include "Enemy.generated.h"

class AEnemySpawner;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyShoot);

UCLASS()
class DIAMONDPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior")
	float MovementSpeed = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Attack")
    float AttackCooldown = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Attack")
    int AttackDamage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Behavior")
	float BaseDetectionRange = 300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Behavior")
	float MaxDetectionRange = 600;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Behavior")
	float DetectionRange;
	
	UPROPERTY(BlueprintReadOnly, Category = "Enemy Behavior");
	bool bCanAttack = true;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Behavior")
	UClass* Projectile;

	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY(EditAnywhere)
	bool bShouldShowDebug = false;
	
	//Garbage
	
	UPROPERTY()
	UBoxComponent* BoxCollider;

	UPROPERTY()
	AAIController* AIController;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnDeath();

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	bool IsPlayerOnSight(FRotator Rotation, FVector Location);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Enemy")
	virtual FRotator GetDirectionRotation(AActor* OriginActor, AActor* TargetActor);
private:
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void DetectPlayer(AActor* Actor);

	UFUNCTION()
	void SetShootReady();

	UFUNCTION()
	void SetNewAttackTimer();

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void Shoot();

	UFUNCTION(BlueprintCallable, Category = "Enemy")

	void RemoveEnemyFromSpawnerList();
	

public:
	
	// Sets default values for this character's properties
	AEnemy();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	UBehaviorTree* GetBehaviorTree() const;

	UPROPERTY()
	TObjectPtr<AEnemySpawner> EnemySpawner = nullptr;

	UPROPERTY(BlueprintAssignable, Category="Sunlight Detection")
	FOnEnemyShoot OnEnemyShoot;

	
};
