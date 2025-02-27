// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEnemy.h"

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
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Bullet"));
	//Take damages player
}

