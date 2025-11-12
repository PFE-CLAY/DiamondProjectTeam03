// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiamondProjectProjectile.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "LDIngredients/BreakableMesh.h"
#include "LoopSystem/AC_Health.h"

ADiamondProjectProjectile::ADiamondProjectProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CollisionComp->InitCapsuleSize(0.5, 5);
	
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ADiamondProjectProjectile::OnHit);		// set up a notification for when this component hits something blocking
	
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

void ADiamondProjectProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if (OtherActor != nullptr && OtherComp != nullptr && Hit.GetActor() != nullptr)
	{
		if (UAC_Health* healthComponent = OtherActor->FindComponentByClass<UAC_Health>()){

			//On peut pas cast en actor
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
