// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiamondProjectProjectile.h"

#include "DiamondProjectCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "LDIngredients/BreakableMesh.h"
#include "LoopSystem/AC_Health.h"

void ADiamondProjectProjectile::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	CollisionComp->OnComponentBeginOverlap.RemoveDynamic(this, &ADiamondProjectProjectile::OnOverlap);
}

ADiamondProjectProjectile::ADiamondProjectProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CollisionComp->InitCapsuleSize(0.5, 5);
	
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	//CollisionComp->OnComponentHit.AddDynamic(this, &ADiamondProjectProjectile::OnHit);		// set up a notification for when this component hits something blocking
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ADiamondProjectProjectile::OnOverlap);
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 6000.f;
	ProjectileMovement->MaxSpeed = 6000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ADiamondProjectProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if (OtherActor != nullptr && OtherComp != nullptr && Hit.GetActor() != nullptr)
	{
		if (Cast<ADiamondProjectCharacter> (OtherActor))
		{
			return;
		}
		
		if (UAC_Health* healthComponent = OtherActor->FindComponentByClass<UAC_Health>()){
			healthComponent->DecreaseHealth(Damage, this);
		}
		else if (ABreakableMesh* Breakable = Cast<ABreakableMesh>(Hit.GetActor()))
		{
			Breakable->OnBreakMesh.Broadcast(Hit.ImpactPoint);
		}
		
		OnHitEvent.Broadcast(Hit);
		if (OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		}

		Destroy();
	}
}

void ADiamondProjectProjectile::Initialize(const float WeaponDamage)
{
	Damage = WeaponDamage;
}
