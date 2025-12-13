// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MeleeEnemy.h"
#include "LoopSystem/AC_Health.h"

// Sets default values
AMeleeEnemy::AMeleeEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();
	OnAttack.AddDynamic(this, &AMeleeEnemy::HitMelee);
	GetWorldTimerManager().SetTimer(InvicibleTimerStart, this, &AMeleeEnemy::SetKillable, 0.2f, false);
}

// Called every frame
void AMeleeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bShouldShowDebug){
		DrawDebugSphere(GetWorld(), RootComponent->GetComponentLocation(), AttackRange, 10, FColor::Red, false, 0.03);
	}
}
// Called to bind functionality to input
void AMeleeEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMeleeEnemy::HitMelee(AActor* Target)
{
	bCanAttack = false;
	OnAttackEvent((AActor*)Target);
	SetNewAttackTimer();
	
}

void AMeleeEnemy::SetKillable()
{
	UAC_Health* HealthComponent = Cast<UAC_Health>(GetComponentByClass(UAC_Health::StaticClass()));
	if(HealthComponent != nullptr)
	{
		HealthComponent->bIsInvincible = false;
	}
}

void AMeleeEnemy::AttackMelee()
{
	Attack(PlayerPawn);
	OnMeleeAttack.Broadcast();
}


bool AMeleeEnemy::IsTargetInRange(AActor* Target)
	{
	float DistanceToPlayer = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
	return (DistanceToPlayer <= AttackRange);
}

void AMeleeEnemy::Charge()
{
	
	GetWorldTimerManager().SetTimer(ChargeTimer, this, &AMeleeEnemy::AttackMelee ,ChargingDuration, false);
	OnChargeEvent.Broadcast();
}


void AMeleeEnemy::CheckDistance()
{
	
	if(FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) >= DistanceCheck)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "AAAAAAAAAAAAAAAAAA");
		KillEnemy();
	}
}

void AMeleeEnemy::CheckDistanceSetTimer()
{
	GetWorldTimerManager().SetTimer(TimerCheckDistance, this, &AMeleeEnemy::CheckDistance, DistanceCheckCooldown, true);
}

void AMeleeEnemy::StopAttack()
{
	GetWorldTimerManager().ClearTimer(ChargeTimer);
	StopAttackEffects();
}

