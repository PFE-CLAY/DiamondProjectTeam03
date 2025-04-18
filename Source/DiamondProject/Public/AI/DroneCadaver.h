// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Allied.h"
#include "GameFramework/Pawn.h"
#include "LDIngredients/HatchIncinerator.h"
#include "DroneCadaver.generated.h"

UCLASS()
class DIAMONDPROJECT_API ADroneCadaver : public APawn
{
	GENERATED_BODY()

private:

	UPROPERTY(Blueprintable)
	AHatchIncinerator* HatchIncinerator;
	
public:
	// Sets default values for this pawn's properties
	ADroneCadaver();

	UPROPERTY()
	TArray<AAllied*> AllAllies;

	UPROPERTY()
	AActor* CarriedBody;

	UPROPERTY(BlueprintReadWrite)
	bool bIsActive;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetDeadBody();

	UFUNCTION(BlueprintCallable)
	void CarryBody();
};
