﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CustomNavigationPoint.h"


// Sets default values
ACustomNavigationPoint::ACustomNavigationPoint()
{

	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACustomNavigationPoint::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void ACustomNavigationPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if(PointType == EPointType::Choice) GEngine->AddOnScreenDebugMessage(666, 5.f, FColor::Blue, FString::Printf(TEXT("AAAAAA = %f"), GetWorldTimerManager().GetTimerElapsed(TimerHandle)));

	
}

void ACustomNavigationPoint::PlayVoiceline()
{
	if(bShouldPlayDialogue) OnPlayDialogue();
}

void ACustomNavigationPoint::StopTimer()
{
	
	if(GetWorldTimerManager().IsTimerActive(TimerHandle)){
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
	PointEffect();
}

void ACustomNavigationPoint::StartTimer()
{
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACustomNavigationPoint::PointEffect, TimeToWait, false);
}



void ACustomNavigationPoint::PointEffect_Implementation()
{
	
	
	switch (PointType){
	case EPointType::None:
		Allied->Patrol();
		break;
	case EPointType::Crouch:
		if(bShouldStayCrouched) Allied->OnCrouch();
		else Allied->OnCrouchTimeLimited(TimeToWaitCrouched);
		break;
	case EPointType::Choice:
		
		Allied->GetNewPath(PathArray[WantedPathIndex]);
		break;
	}
	
}






