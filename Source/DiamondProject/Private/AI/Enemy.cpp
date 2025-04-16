// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemy.h"

#include "AI/ProjectileEnemy.h"
#include "Components/BoxComponent.h"
#include "DiamondProject/DiamondProjectCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


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
	if(bShouldShowDebug){
		DrawDebugSphere(GetWorld(), RootComponent->GetComponentLocation(), DetectionRange, 50, FColor::Blue, false, 0.03);
	}
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
	
	if(bCanAttack){
		FVector const Location = GetActorLocation();
		FRotator const Rotation = (PlayerPawn->GetActorLocation() - GetActorLocation()).Rotation();
		bCanAttack = false;
		FActorSpawnParameters SpawnInfo;
		AActor* ProjectileSpawned = GetWorld()->SpawnActor(Projectile, &Location, &Rotation, SpawnInfo);
		if(ProjectileSpawned == nullptr){
			return;
		}
		AProjectileEnemy* ProjectileInstance = Cast<AProjectileEnemy>(ProjectileSpawned);
		ProjectileInstance->ProjectileDamage = AttackDamage;
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Purple, "aaaaaaaaaaaaaaaaa");
		SetNewAttackTimer();
		
		
	}
}

bool AEnemy::IsPlayerOnSight(FRotator Rotation, FVector Location)
{
	bool bIsPlayerOnSight = false;
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	
	CollisionParams.AddIgnoredActor(this);
	FVector End = GetActorLocation() + (Rotation.Vector() * DetectionRange);
	bool bHasHit = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_Camera, CollisionParams);
	//DrawDebugLine(GetWorld(), Location, End, bHasHit? FColor::Red : FColor::Green, false, 0.3f, 0, 10.f);
	bIsPlayerOnSight = (bHasHit && Hit.GetActor() == PlayerPawn);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Is Player On Sight: %s"), bIsPlayerOnSight ? TEXT("true") : TEXT("false")));
	return bIsPlayerOnSight;
}

FRotator AEnemy::GetDirectionRotation(AActor* OriginActor, AActor* TargetActor)
{
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(OriginActor->GetActorLocation(),
		TargetActor->GetActorLocation());
	return Rotation;
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




