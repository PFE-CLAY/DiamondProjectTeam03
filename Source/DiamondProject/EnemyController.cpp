// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Enemy.h"

// Sets default values
AEnemyController::AEnemyController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	
	Super::OnPossess(InPawn);
	if(const AEnemy* enemy = Cast<AEnemy>(InPawn)){
		if(UBehaviorTree* const BehaviorTree = enemy->GetBehaviorTree()){
			UBlackboardComponent* BlackboardComponent;
			UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent);
			Blackboard = BlackboardComponent;
			RunBehaviorTree(BehaviorTree);
			
		}
	}
	
}

