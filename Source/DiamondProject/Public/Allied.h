// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Allied.generated.h"

UCLASS()
class DIAMONDPROJECT_API AAllied : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	AAIController* AIController;

	UPROPERTY(EditAnywhere, Category = "Movements")
	TArray<AActor*> PatrolPoints;

	UPROPERTY()
	int Position = 0;
	
public:
	// Sets default values for this character's properties
	AAllied();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Patrol();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
