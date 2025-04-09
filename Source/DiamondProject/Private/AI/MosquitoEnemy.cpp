// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MosquitoEnemy.h"

#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AMosquitoEnemy::AMosquitoEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMosquitoEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxFlySpeed = MoveSpeed;
}

// Called every frame
void AMosquitoEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMosquitoEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

