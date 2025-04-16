// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AlliedAIController.h"

#include "AI/Allied.h"


// Sets default values
AAlliedAIController::AAlliedAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAlliedAIController::BeginPlay()
{
	Super::BeginPlay();
	AlliedControlled = Cast<AAllied>(GetPawn());
	
	
	
}

void AAlliedAIController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	AlliedControlled->Position++;
	if(AlliedControlled->bShouldLoop && AlliedControlled->Position >= AlliedControlled->PatrolPoints.Num()){
		AlliedControlled->Position = 0;
	}
	AlliedControlled->Patrol();
	
}

// Called every frame
void AAlliedAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

