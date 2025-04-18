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


	
	
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Movements")
	TArray<TObjectPtr<AActor>> PatrolPoints;
	// Sets default values for this character's properties
	AAllied();

	UPROPERTY()
	int Position = 0;

	UPROPERTY(EditAnywhere, Category = "Movements")
	bool bShouldLoop = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsDead = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Patrol();
};
