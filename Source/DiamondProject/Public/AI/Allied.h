﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AlliedAnimInstance.h"

#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Allied.generated.h"

UCLASS()
class DIAMONDPROJECT_API AAllied : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	AAIController* AIController;


	
	
public:
	UPROPERTY(EditInstanceOnly, Category="Path")
	APath* Path;
	// Sets default values for this character's properties
	AAllied();

	UPROPERTY()
	int PositionID = 0;

	UPROPERTY(EditAnywhere, Category = "Movements")
	bool bShouldLoop = false;

	UPROPERTY(EditAnywhere, Category = "Grab")
	FVector GrabLocation;
	
	UPROPERTY(EditAnywhere, Category = "Grab")
	FName GrabBoneName = "Root";

	UPROPERTY(BlueprintReadWrite)
	bool bIsDead = false;

	UPROPERTY(BlueprintReadWrite)
	USkeletalMeshComponent* AlliedMesh;

	UPROPERTY(BlueprintReadWrite)
	UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY()
	UAlliedAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* GrabPoint;

	UPROPERTY(EditAnywhere, Category = "Movements")
	bool bShouldPlayOnStart = false;

	UPROPERTY()
	UCapsuleComponent* CapsuleAllied;

	UPROPERTY()
	ACustomNavigationPoint* LastNavigationPoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category="Movements")
	void Patrol();

	UFUNCTION()
	ACustomNavigationPoint* GetCurrentNavigationPoint();

	UFUNCTION()
	void GetNewPath(APath* NewPath);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Movements")
	void OnCrouch();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Movements")
	void OnUncrouch();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Movements")
	void OnCrouchTimeLimited(float Duration);
};
