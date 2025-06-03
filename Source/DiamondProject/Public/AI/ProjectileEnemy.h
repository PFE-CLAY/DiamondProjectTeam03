// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ProjectileEnemy.generated.h"

UCLASS()
class DIAMONDPROJECT_API AProjectileEnemy : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	UStaticMeshComponent* MeshCollider;

	
public:
	// Sets default values for this actor's properties
	UPROPERTY()
	int ProjectileDamage = 0;
	
	AProjectileEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCollision(AActor* OverlappedActor, AActor* OtherActor);

	
};
