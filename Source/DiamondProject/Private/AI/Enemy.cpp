// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Enemy.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	AIController = this->GetController<AAIController>();
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if(BehaviorTree != nullptr) AIController->RunBehaviorTree(BehaviorTree);
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	GetCharacterMovement()->MaxFlySpeed = Speed;
}

void AEnemy::OnDeath()
{
	K2_DestroyActor();
}

void AEnemy::SetNewAttackTimer()
{
	GetWorldTimerManager().SetTimer(TimerHandle, this,  &AEnemy::SetAttackReady, AttackCooldown, false);
}

void AEnemy::Attack(AActor* Target)
{
	if(bCanAttack)
	{
		OnAttack.Broadcast(Target);
	}
	
}

void AEnemy::SetAttackReady()
{
	bCanAttack = true;
}


// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FRotator AEnemy::GetDirectionRotation(AActor* OriginActor, AActor* TargetActor)
{
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(OriginActor->GetActorLocation(),
		TargetActor->GetActorLocation());
	return Rotation;
}



