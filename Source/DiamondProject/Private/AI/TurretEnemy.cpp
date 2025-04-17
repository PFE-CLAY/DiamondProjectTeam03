// Fill out your copyright notice in the Description page of Project Settings.


#include "DiamondProject/Public/AI/TurretEnemy.h"


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

FRotator ATurretEnemy::GetDirectionRotation(AActor* OriginActor, AActor* TargetActor)
{
	FRotator Result = Super::GetDirectionRotation(OriginActor, TargetActor);
	FRotator Rotation = FRotator(0, Result.Yaw, 0);
	//GEngine->AddOnScreenDebugMessage(15, 1.f, FColor::Purple, FString::Printf(TEXT("Vector: %f %f %f"), Rotation.Pitch, Rotation.Yaw, Rotation.Roll));
	return Rotation;
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

