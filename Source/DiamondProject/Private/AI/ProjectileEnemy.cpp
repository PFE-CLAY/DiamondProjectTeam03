﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ProjectileEnemy.h"

#include "AI/Enemy.h"
#include "LoopSystem/AC_Health.h"
#include "DiamondProject/DiamondProjectCharacter.h"
#include "Engine/Engine.h"

// Sets default values
AProjectileEnemy::AProjectileEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AProjectileEnemy::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AProjectileEnemy::OnCollision);
	
}

// Called every frame
void AProjectileEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileEnemy::OnCollision(AActor* OverlappedActor, AActor* OtherActor)
{
	
	if(Cast<AEnemy>(OtherActor) == nullptr){
		
		if (UAC_Health* HealthComponent = OtherActor->FindComponentByClass<UAC_Health>()) {
			HealthComponent->DecreaseHealth(ProjectileDamage, this);
		}
		
		OnProjectileDestroyed();
		return;
	}
	else if (Cast<AEnemy>(OtherActor)){
		return;
	}
	
	OnProjectileDestroyed();
}
