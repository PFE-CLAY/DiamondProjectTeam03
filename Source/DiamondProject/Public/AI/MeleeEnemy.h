// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "MeleeEnemy.generated.h"

UCLASS()
class DIAMONDPROJECT_API AMeleeEnemy : public AEnemy
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,  Category = "Attack")
	float AttackRange = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackDuration = 0.5f;

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	bool bIsAttacking = true;

	UPROPERTY()
	FTimerHandle InvicibleTimerStart;
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
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsTargetInRange(AActor* Target);
	UFUNCTION(BlueprintImplementableEvent)
	void OnAttackEvent(AActor* Target);
};
