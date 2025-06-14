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
	UDELEGATE(BlueprintCallable)
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedEvent, int, CurrentHealth, const AActor*, DamageDealer);

	UDELEGATE(BlueprintCallable)
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathEvent, const AActor*, DamageDealer);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaxHealth = 3;

	UPROPERTY(BlueprintReadOnly)
	int CurrentHealth;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedEvent OnHealthChangedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnDeathEvent OnDeathEvent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInvincible = false;

protected:
	UPROPERTY(BlueprintReadOnly)
    bool bIsDead;
	
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
    TArray<TSubclassOf<AActor>> DamageIgnoreClasses;

public:
	// Sets default values for this component's properties
	UAC_Health();

	UFUNCTION(BlueprintCallable)
	void DecreaseOneHealth();

	UFUNCTION(BlueprintCallable)
	void DecreaseHealth(int Damage, const AActor* DamageDealer = nullptr);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION()
	void ChangeHealth(int NewHealth, const AActor* DamageDealer = nullptr);

	UFUNCTION(BlueprintCallable)
	void FullHeal();
};
