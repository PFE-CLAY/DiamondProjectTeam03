// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_Health.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIAMONDPROJECT_API UAC_Health : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	int MaxHealth = 3;

	UPROPERTY()
	int CurrentHealth;
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedEvent, int, CurrentHealth);
	UPROPERTY()
	FOnHealthChangedEvent OnHealthChangedEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathEvent);
	UPROPERTY()
	FOnDeathEvent OnDeathEvent;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UAC_Health();

	UFUNCTION(BlueprintCallable)
	void DecreaseOneHealth();

	UFUNCTION(BlueprintCallable)
	void DecreaseHealth(int Damage);

	UFUNCTION()
	void ChangeHealth(int NewHealth);
};
