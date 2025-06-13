// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemy.h"
#include "Components/BoxComponent.h"
#include "DiamondProject/DiamondProjectCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI/EnemySpawner.h"
#include "AI/ProjectileEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "LoopSystem/AC_Health.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* ShootPoint = CreateDefaultSubobject<USceneComponent>("ShootPoint");
	ShootPoint->SetupAttachment(GetMesh());
	ShootPoints.Add(ShootPoint);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	AIController = this->GetController<AAIController>();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAllied::StaticClass(), AllTargetActors);
	AllTargetActors.Add(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
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

AActor* AEnemy::GetClosestAliveTarget()
{
	float ClosestRange = -1;
	AActor* ClosestTarget = nullptr;
	if(AllTargetActors.Num() <= 0) return nullptr;
	for(AActor* Target : AllTargetActors){
		if (Target->FindComponentByClass<UAC_Health>()->CurrentHealth == 0) continue;
		float Distance = FVector::Distance(Target->GetActorLocation(), GetActorLocation());
		if(Distance < ClosestRange || ClosestRange == -1){
			ClosestTarget = Target;
			ClosestRange = Distance;
		}
	}
	return ClosestTarget;
}

bool AEnemy::IsAnyTargetInRange()
{
	
	int TargetNumber = 0;
	for(AActor* Target : AllTargetActors){
		FVector Direction = Target->GetActorLocation() - GetActorLocation();
		FRotator LookAtRotation;
		LookAtRotation.Yaw = FMath::RadiansToDegrees(FMath::Atan2(Direction.Y, Direction.X)) ;
		LookAtRotation.Pitch = FMath::RadiansToDegrees(FMath::Atan2(Direction.Z, FVector2D(Direction.X, Direction.Y).Size()));
		LookAtRotation.Roll = 0;
		float Distance = FVector::Distance(Target->GetActorLocation(), GetActorLocation());
		if(IsTargetOnSight(LookAtRotation, Target->GetActorLocation()) && Distance < DetectionRange){
			TargetNumber += 1;
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT(" Number of targets: %d"), TargetNumber));
	if(TargetNumber > 0){
		return true;
	}
	return false;
	
}

USceneComponent* AEnemy::GetNextShootPoint()
{
	USceneComponent* shootPoint = nullptr;
	if (ShootPoints.Num() > 0)
	{
		IndexShootPoint = (IndexShootPoint + 1) % ShootPoints.Num();
		shootPoint = ShootPoints[IndexShootPoint];
	}
	return shootPoint;
}

USceneComponent* AEnemy::GetCurrentShootPoint()
{
	USceneComponent* shootPoint = nullptr;
	if (ShootPoints.Num() > 0)
	{
		shootPoint = ShootPoints[IndexShootPoint];
	}
	return shootPoint;
}

void AEnemy::AddShootPoint(USceneComponent* ShootPoint)
{
	if (ShootPoint && !ShootPoints.Contains(ShootPoint))
	{
		ShootPoints.Add(ShootPoint);
	}
}

void AEnemy::RemoveShootPoint(USceneComponent* ShootPoint)
{
	if (ShootPoint)
	{
		ShootPoints.Remove(ShootPoint);
	}
}

void AEnemy::Shoot(AActor* Target)
{
	
	if(bCanAttack)
	{
		
		USceneComponent* ShootPoint = GetNextShootPoint();
		if (ShootPoint != nullptr)
		{
			FVector const Location = ShootPoint->GetComponentLocation();
			FRotator const Rotation = (Target->GetActorLocation() - ShootPoint->GetComponentLocation()).Rotation();
			bCanAttack = false;
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Name = "aze";
			AActor* ProjectileSpawned = nullptr;
			if (GetWorld()) {
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "SKIBIDI");
				ProjectileSpawned = GetWorld()->SpawnActor(Projectile, &Location, &Rotation);
				OnEnemyShoot.Broadcast();
			}
			

			if(ProjectileSpawned == nullptr){
				
				bCanAttack = true;
				return;
			}
			
			AProjectileEnemy* ProjectileInstance = Cast<AProjectileEnemy>(ProjectileSpawned);
			ProjectileInstance->ProjectileDamage = AttackDamage;
		
			SetNewAttackTimer();
		}
	}
}

void AEnemy::RemoveEnemyFromSpawnerList()
{
	/*EnemySpawner->SpawnedEnemies.Remove(this);*/
}

bool AEnemy::IsTargetOnSight(FRotator Rotation, FVector Location)
{
	bool bIsPlayerOnSight = false;
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	
	CollisionParams.AddIgnoredActor(this);
	FVector End = GetActorLocation() + (Rotation.Vector() * DetectionRange);
	bool bHasHit = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_Camera, CollisionParams);
	//DrawDebugLine(GetWorld(), Location, End, bHasHit? FColor::Red : FColor::Green, false, 0.3f, 0, 10.f);
	bIsPlayerOnSight = (bHasHit && (Hit.GetActor() == PlayerPawn || Cast<AAllied>(Hit.GetActor()) != nullptr));
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
	GetWorldTimerManager().SetTimer(TimerHandle, this,  &AEnemy::SetShootReady, AttackCooldown, false);
}

void AEnemy::SetShootReady()
{
	bCanAttack = true;	
}





