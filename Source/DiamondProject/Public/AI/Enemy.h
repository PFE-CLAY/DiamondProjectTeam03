// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Components/BoxComponent.h"
#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttack, AActor*, Target);

UCLASS()
class DIAMONDPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadOnly, Category = "Attack");
	bool bCanAttack = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackCooldown = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int AttackDamage = 1;

	UPROPERTY(BlueprintAssignable, Category="Attack")
	FOnAttack OnAttack;

	UPROPERTY()
	FTimerHandle TimerHandle;
	
	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY()
	UBoxComponent* BoxCollider;

	UPROPERTY()
	AAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior")
	float Speed = 600;

	UPROPERTY(EditAnywhere, Category = "Attack")
	bool bShouldShowDebug = false;
	
	

	
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
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void Attack(AActor* Target);
	UFUNCTION()
	void SetAttackReady();

	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Enemy")
    virtual FRotator GetDirectionRotation(AActor* OriginActor, AActor* TargetActor);
	
};
