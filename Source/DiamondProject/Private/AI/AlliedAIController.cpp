// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AlliedAIController.h"

#include "AI/Allied.h"
#include "AI/CustomNavigationPoint.h"
#include "AI/Path.h"
#include "Navigation/PathFollowingComponent.h"


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
	UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPathFollowingResult"), true);
	if (EnumPtr)
	{
		FString EnumName = EnumPtr->GetNameStringByValue(static_cast<int64>(Result));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, EnumName);
	}
	AlliedControlled->LastNavigationPoint = AlliedControlled->GetCurrentNavigationPoint();
	AlliedControlled->LastNavigationPoint->OnArrivingOnPoint();
	if(AlliedControlled->LastNavigationPoint == nullptr) return;
	AlliedControlled->PositionID++;
	AlliedControlled->LastNavigationPoint->PlayVoiceline();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("AAAAAA = %d"), AlliedControlled->PositionID));

	if(AlliedControlled->LastNavigationPoint->bShouldWait){
		AlliedControlled->LastNavigationPoint->StartTimer();
		return;
	}
	
	AlliedControlled->LastNavigationPoint->PointEffect();
	if(AlliedControlled->LastNavigationPoint->PointType == EPointType::Crouch) return;
	if(AlliedControlled->PositionID >= AlliedControlled->Path->PatrolPoints.Num())
	{
		if(AlliedControlled->bShouldLoop){
			AlliedControlled->PositionID = 0;
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







