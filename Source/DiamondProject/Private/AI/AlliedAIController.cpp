﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AlliedAIController.h"

#include "AI/Allied.h"
#include "AI/CustomNavigationPoint.h"
#include "AI/Path.h"


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
	
	ACustomNavigationPoint* CurrentNavigationPoint = AlliedControlled->GetCurrentNavigationPoint();
	if(CurrentNavigationPoint == nullptr) return;
	
	AlliedControlled->Position++;

	if(CurrentNavigationPoint->bShouldWait){
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, CurrentNavigationPoint, &ACustomNavigationPoint::PointEffect, CurrentNavigationPoint->TimeToWait, false);
		return;
	}
	CurrentNavigationPoint->PointEffect();
	if(CurrentNavigationPoint->PointType == EPointType::Crouch) return;
	if(AlliedControlled->Position >= AlliedControlled->Path->PatrolPoints.Num())
	{
		if(AlliedControlled->bShouldLoop){
			AlliedControlled->Position = 0;
		}
		else return;
	}
	
	
}

void AAlliedAIController::StartPatrol()
{
	AlliedControlled->Patrol();
}


// Called every frame
void AAlliedAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}







