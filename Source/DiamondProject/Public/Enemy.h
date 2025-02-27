// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Components/BoxComponent.h"
#include "Enemy.generated.h"



UCLASS()
class DIAMONDPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior")
	float MovementSpeed = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior")
    float AttackCooldown = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Behavior")
	float BaseDetectionRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Behavior")
	float MaxDetectionRange;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Behavior")
	float DetectionRange;
	
	
	UPROPERTY(BlueprintReadOnly, Category = "Enemy Behavior");
	bool bCanAttack = true;

	UPROPERTY(EditDefaultsOnly, Category = "Turret Behavior")
	UClass* Projectile;

	UPROPERTY()
	APawn* PlayerPawn;
	
	//Garbage
	
	UPROPERTY()
	UBoxComponent* BoxCollider;

	UPROPERTY()
	AAIController* AIController;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void DetectPlayer(AActor* Actor);

	UFUNCTION()
	void SetShootReady();

	UFUNCTION()
	void SetNewAttackTimer();

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void Shoot();

public:
	
	// Sets default values for this character's properties
	AEnemy();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	UBehaviorTree* GetBehaviorTree() const;

	
};
