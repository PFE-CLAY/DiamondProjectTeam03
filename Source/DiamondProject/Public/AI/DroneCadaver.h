// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Allied.h"
#include "GameFramework/Pawn.h"
#include "Runtime/Core/Public/Containers/Queue.h"
#include "LDIngredients/HatchIncinerator.h"
#include "DroneCadaver.generated.h"

UCLASS()
class DIAMONDPROJECT_API ADroneCadaver : public APawn
{
	GENERATED_BODY()

private:

	

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* GrabPoint;

	UPROPERTY()
	bool bShouldGrab = false;
	
public:
	// Sets default values for this pawn's properties
	ADroneCadaver();

	UPROPERTY(BlueprintReadOnly)
	AHatchIncinerator* HatchIncinerator;

	

	UPROPERTY()
	TArray<AAllied*> AllAllies;

	UPROPERTY()
	AAllied* TargetBody;

	UPROPERTY(BlueprintReadWrite)
	bool bIsActive;
	
	//TQueue<AAllied*> CadaverQueue;
	
	UPROPERTY()
	TArray<AAllied*> CadaverArray;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
	void OnMovingToNewCadaver(AAllied* Target);

	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
	void GoOutHatch();

	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
	void GoInHatch();

	UFUNCTION(BlueprintCallable)
	void GoToNewTarget();

	UFUNCTION(BlueprintCallable)
	void CarryBody();

	UFUNCTION(BlueprintCallable)
	void ThrowBody();

	UFUNCTION()
	void UpdateTargetLocation();

	UFUNCTION(BlueprintCallable)
	void GetNewCadaver(AAllied* Allied);
};
