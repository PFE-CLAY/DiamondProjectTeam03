// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class DIAMONDPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Behavior");
	bool bCanAttack = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Attack")
	float AttackCooldown = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Attack")
	int AttackDamage = 1;

	UPROPERTY()
	FTimerHandle TimerHandle;
	
	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY()
	UBoxComponent* BoxCollider;

	UPROPERTY()
	AAIController* AIController;
public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void OnDeath();
	UFUNCTION()
	void SetNewAttackTimer();

	UFUNCTION()
	void SetAttackReady();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
