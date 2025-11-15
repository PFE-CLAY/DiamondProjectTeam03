// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DiamondProjectProjectile.generated.h"

class UCapsuleComponent;
class UProjectileMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitEvent, FHitResult, HitInfo);

UCLASS(config=Game)
class ADiamondProjectProjectile : public AActor
{
	GENERATED_BODY()

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	UCapsuleComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	float Damage;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHitEvent OnHitEvent;

	ADiamondProjectProjectile();

	/** called when projectile hits something */
	//UFUNCTION()
	//void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void Initialize(const float WeaponDamage);
	
	/** Returns CollisionComp subobject **/
	UCapsuleComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

