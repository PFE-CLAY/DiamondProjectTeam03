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
	ChangeHealth(CurrentHealth - 1, nullptr);
}

void UAC_Health::DecreaseHealth(int Damage, const AActor* DamageDealer) 
{
	if (!DamageDealer) {
		UE_LOG(LogTemp, Error, TEXT("NO DAMAGE DEALER PROVIDED!"));
		return;
	}

	for (const auto& IgnoredClass : DamageIgnoreClasses) {
		if (DamageDealer->IsA(IgnoredClass)) {
			return;
		}
	}
	
	ChangeHealth(CurrentHealth - Damage, DamageDealer);
}

void UAC_Health::ChangeHealth(int NewHealth, const AActor* DamageDealer)
{
	if (bIsInvincible && NewHealth > CurrentHealth) {
		return;
	}
	
	NewHealth = FMath::Clamp(NewHealth, 0, MaxHealth);
	CurrentHealth = NewHealth;
	
	if (CurrentHealth == 0 && !bIsDead) {
		OnDeathEvent.Broadcast(DamageDealer);
		bIsDead = true;
	}
	
	OnHealthChangedEvent.Broadcast(CurrentHealth, DamageDealer);
}

void UAC_Health::FullHeal()
{
	ChangeHealth(MaxHealth);
}
