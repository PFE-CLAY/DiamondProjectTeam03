// Fill out your copyright notice in the Description page of Project Settings.


#include "Allied.h"


// Sets default values
AAllied::AAllied()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AAllied::BeginPlay()
{
	Super::BeginPlay();
	AIController = this->GetController<AAIController>();
	//AAIController::OnMoveCompleted();
	
}

void AAllied::Patrol()
{
	AIController->MoveToActor(PatrolPoints[Position], -1, true, false);
}

// Called every frame
void AAllied::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAllied::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

