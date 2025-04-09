// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "MosquitoEnemy.generated.h"

UCLASS()
class DIAMONDPROJECT_API AMosquitoEnemy : public AEnemy
{
	GENERATED_BODY()



public:
	// Sets default values for this character's properties
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Mosquito")
	float MoveSpeed = 800;
	
	AMosquitoEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
