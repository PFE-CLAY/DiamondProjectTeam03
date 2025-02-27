// Fill out your copyright notice in the Description page of Project Settings.


#include "DiamondProject/Public/LoopSystem/AC_Health.h"

// Sets default values for this component's properties
UAC_Health::UAC_Health()
{
}

// Called when the game starts
void UAC_Health::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void UAC_Health::DecreaseOneHealth()
{
	ChangeHealth(CurrentHealth - 1);
}

void UAC_Health::DecreaseHealth(int Damage) 
{
	ChangeHealth(CurrentHealth - Damage);
}

void UAC_Health::ChangeHealth(int NewHealth)
{
	NewHealth = FMath::Clamp(NewHealth, 0, MaxHealth);
	CurrentHealth = NewHealth;

	if (CurrentHealth == 0 && !bIsDead) {
		OnDeathEvent.Broadcast();
		bIsDead = true;
	} else {
		OnHealthChangedEvent.Broadcast(CurrentHealth);
	}
}
