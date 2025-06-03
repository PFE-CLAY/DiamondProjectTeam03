// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Queue.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoopSubsystem.generated.h"

class UPreplanDataWidget;
class UPreplanStep;

/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API ULoopSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY()
	bool bIsInit = false;

	UDELEGATE(BlueprintCallable)
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSceneReloadEvent);

public:
	UPROPERTY(BlueprintReadWrite)
	bool bIsDreamLevel = false;
	
	UPROPERTY(BlueprintAssignable)
	FOnSceneReloadEvent OnSceneReloadEvent;
	
	UPROPERTY()
	TMap<FString, TObjectPtr<UPreplanStep>> PreplanSteps;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UDataTable>> PreplanDreamSubtitlesArray;

private:
	UFUNCTION(BlueprintCallable)
	void ReloadScene();

	UFUNCTION()
	bool IsAnyPreviousStepActive(const UPreplanStep* PreplanStep);

	UFUNCTION()
	void InitializePreplanSteps();
	
	UFUNCTION()
	void InitializePreplanAdvices();

	UFUNCTION()
	void InitializePreplanLinks();

	UFUNCTION()
	void SetPreplanVisibility(UPreplanDataWidget* PreplanData, bool bIsVisible);

	UFUNCTION()
	void CreatePreplanStep(UPreplanDataWidget* PreplanDataWidget);

public:
	UFUNCTION()
	void OnAdvicesVisibilityChanged(bool bNewVisibility);
	
	UFUNCTION(BlueprintCallable)
	void InitializePreplan();

	UFUNCTION(BlueprintCallable)
	void ActivatePreplanStep(FString PreplanID);
};
