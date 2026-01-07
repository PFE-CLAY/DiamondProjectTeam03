// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Enemy.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


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
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();;
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	GetCharacterMovement()->MaxFlySpeed = Speed;
	if(bIsActiveAtStart) StartBehavior();
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
	if(bCanAttack && Target && IsTargetInRange(Target))
	{
		OnAttack.Broadcast(Target);
	}
	
}

void AEnemy::SetAttackReady()
{
	bCanAttack = true;
}

void AEnemy::StartBehavior_Implementation()
{
	if(BehaviorTree != nullptr) AIController->RunBehaviorTree(BehaviorTree);
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

bool AEnemy::IsTargetInRange(AActor* Target)
{
	if (Target == nullptr)
	{
		return false;
	}

	// If a derived class defines its own notion of range, it can override this method.
	// By default, only line-of-sight check is enforced here.
	FHitResult HitResult;
	FCollisionQueryParams QueryParams(TEXT("EnemyBaseLineTrace"), /*bTraceComplex*/ false, this);
	QueryParams.AddIgnoredActor(this);

	const FVector Start = GetActorLocation();
	const FVector End = Target->GetActorLocation();

	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);

	if (bShouldShowDebug)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 0.1f, 0, 1.5f);
	}

	if (!bHit)
	{
		return true; // nothing blocking
	}

	AActor* HitActor = HitResult.GetActor();
	if (HitActor == nullptr)
	{
		return true; // unexpected but treat as clear
	}

	// Allow if we directly hit the intended target
	if (HitActor == Target)
	{
		return true;
	}

	// Allow if we hit any enemy (friendly fire/stacking not blocked by LOS)
	if (HitActor->IsA(AEnemy::StaticClass()))
	{
		return true;
	}

	// Otherwise, something else is in the way
	return false;
}
