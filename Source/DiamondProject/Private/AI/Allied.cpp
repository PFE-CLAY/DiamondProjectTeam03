﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Allied.h"
#include "AIController.h"

#include "CookOnTheFly.h"
#include "AI/CustomNavigationPoint.h"
#include "AI/Path.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"


// Sets default values
AAllied::AAllied()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>("PhysicsHandle");
	GrabPoint = CreateDefaultSubobject<USceneComponent>("GrabPoint");
	GrabPoint->SetupAttachment(GetMesh());
	
}

// Called when the game starts or when spawned
void AAllied::BeginPlay()
{
	Super::BeginPlay();
	AlliedMesh = Cast<USkeletalMeshComponent>(GetMesh());

	if(Path != nullptr) Path->SetAllied(this);
	
	if (AlliedMesh != nullptr){
		AnimInstance = Cast<UAlliedAnimInstance>(AlliedMesh->GetAnimInstance());
	}
	AIController = this->GetController<AAIController>();
	
	//AAIController::OnMoveCompleted();
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("%lld"), PatrolPoints.Num()));
	if(bShouldPlayOnStart) Patrol();
}

void AAllied::Patrol()
{
	if(Path->PatrolPoints.IsEmpty()) return;
	if(Position < Path->PatrolPoints.Num()){
		AnimInstance->bIsMoving = true;
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalLocation(Path->PatrolPoints[Position]->GetActorLocation());
		AIController->MoveTo(MoveRequest, nullptr);
	}
	else
	{
		AnimInstance->bIsMoving = false;
	}
	
	
}

ACustomNavigationPoint* AAllied::GetCurrentNavigationPoint()
{
	
	if(Path->PatrolPoints[Position] != nullptr){ return Path->PatrolPoints[Position];}
	return nullptr;
}

void AAllied::GetNewPath(APath* NewPath)
{
	Position = 0;
	Path = NewPath;
	Path->SetAllied(this);
	Patrol();
}

void AAllied::OnCrouchTimeLimited_Implementation(float Duration)
{
	GetCharacterMovement()->DisableMovement();
}

void AAllied::OnCrouch_Implementation()
{
	GetCharacterMovement()->DisableMovement();
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



