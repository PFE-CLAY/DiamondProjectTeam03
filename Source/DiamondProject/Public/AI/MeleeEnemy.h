// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "MeleeEnemy.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharge);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMeleeAttack);
UCLASS()
class DIAMONDPROJECT_API AMeleeEnemy : public AEnemy
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,  Category = "Attack")
	float AttackRange = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackDuration = 0.5f;

	UPROPERTY(BlueprintReadWrite, Category = "Attack Melee")
	float ChargingDuration = 2.f;
	
	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	bool bIsAttacking = true;

	UPROPERTY(BlueprintAssignable, Category="Attack Melee")
	FOnCharge OnChargeEvent;

	UPROPERTY(BlueprintAssignable, Category="Attack")
	FOnMeleeAttack OnMeleeAttack;

	UPROPERTY()
	FTimerHandle InvicibleTimerStart;

	UPROPERTY()
	FTimerHandle ChargeTimer;

	UPROPERTY(EditAnywhere,  Category = "Behavior")
	float DistanceCheckCooldown = 7.f;

	UPROPERTY(EditAnywhere,  Category = "Behavior")
	float DistanceCheck = 5000.f;

	UPROPERTY()
	FTimerHandle TimerCheckDistance;
public:
	// Sets default values for this character's properties
	AMeleeEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	void HitMelee(AActor* Target);

	UFUNCTION()
	void SetKillable();

	UFUNCTION()
	void AttackMelee();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsTargetInRange(AActor* Target);
	UFUNCTION(BlueprintImplementableEvent)
	void OnAttackEvent(AActor* Target);

	UFUNCTION(BlueprintCallable)
	void Charge();

	UFUNCTION(BlueprintImplementableEvent)
	void KillEnemy();
	
	UFUNCTION()
	void CheckDistance();

	UFUNCTION(BlueprintCallable)
	void CheckDistanceSetTimer();

	UFUNCTION(BlueprintCallable)
	void StopAttack();

	UFUNCTION(BlueprintImplementableEvent)
	void StopAttackEffects();
};
