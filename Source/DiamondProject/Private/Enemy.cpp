// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "ProjectileEnemy.h"
#include "Components/BoxComponent.h"
#include "DiamondProject/DiamondProjectCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


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
	DetectionRange = BaseDetectionRange;
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	
}

void AEnemy::OnDeath()
{
	K2_DestroyActor();
}

void AEnemy::DetectPlayer(AActor* Actor)
{
	if(ADiamondProjectCharacter* PlayerCharacter = Cast<ADiamondProjectCharacter>(Actor)){
		if(bCanAttack){
			//hit
			
		}
			
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugSphere(GetWorld(), RootComponent->GetComponentLocation(), DetectionRange, 50, FColor::Blue, false, 0.03);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UBehaviorTree* AEnemy::GetBehaviorTree() const
{
	return BehaviorTree;
}



void AEnemy::Shoot()
{
	if(bCanAttack)
	{
		bCanAttack = false;
		FVector const Location = GetActorLocation();
		FRotator const Rotation = (PlayerPawn->GetActorLocation() - GetActorLocation()).Rotation();
		FActorSpawnParameters SpawnInfo;
		AActor* ProjectileSpawned = GetWorld()->SpawnActor(Projectile, &Location, &Rotation, SpawnInfo);
		Cast<AProjectileEnemy>(ProjectileSpawned)->ProjectileDamage = AttackDamage;
		
		SetNewAttackTimer();
		
	}
}

void AEnemy::SetNewAttackTimer()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this,  &AEnemy::SetShootReady, AttackCooldown, false);
}

void AEnemy::SetShootReady()
{
	bCanAttack = true;
}




