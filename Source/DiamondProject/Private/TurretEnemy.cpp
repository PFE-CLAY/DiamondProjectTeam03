// Fill out your copyright notice in the Description page of Project Settings.


#include "DiamondProject/Public/TurretEnemy.h"


// Sets default values
ATurretEnemy::ATurretEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATurretEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATurretEnemy::StartPatrol()
{
	
	FVector NextPoint = PatrolPoints[Position]->GetActorLocation();
	AIController->MoveToLocation(NextPoint);
	Position++;
	if(Position == PatrolPoints.Num())
	{
		Position = 0;
	}
	
}

// Called every frame
void ATurretEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATurretEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

