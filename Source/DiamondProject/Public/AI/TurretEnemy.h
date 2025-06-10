// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "TurretEnemy.generated.h"

UCLASS()
class DIAMONDPROJECT_API ATurretEnemy : public AEnemy
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Behavior")
	TArray<AActor*> PatrolPoints;

	UPROPERTY(BlueprintReadWrite, Category = "Turret Behavior")
	int Position;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual FRotator GetDirectionRotation(AActor* OriginActor, AActor* TargetActor) override;

private:
	UFUNCTION(BlueprintCallable)
	void StartPatrol();
public:
	ATurretEnemy();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
