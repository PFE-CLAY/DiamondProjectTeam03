// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Queue.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoopSubsystem.generated.h"

class UPreplanDataWidget;
class UPreplanStep;
class UCollectible;
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

	UPROPERTY()
	bool bIsCollectibleInit = false;

	UDELEGATE(BlueprintCallable)
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSceneReloadEvent);

public:
	UPROPERTY(BlueprintReadWrite)
	bool bIsDreamLevel = false;

	UPROPERTY(BlueprintReadOnly)
	int LoopNb = 0;

	UPROPERTY(BlueprintReadWrite)
	bool bIsDeadByTimer = false;
	
	UPROPERTY(BlueprintAssignable)
	FOnSceneReloadEvent OnSceneReloadEvent;
	
	UPROPERTY()
	TMap<FString, TObjectPtr<UPreplanStep>> PreplanSteps;

	UPROPERTY()
	TMap<FString,bool> Collectibles;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UDataTable>> PreplanDreamSubtitlesArray;

private:
	UFUNCTION(BlueprintCallable)
	void ReloadScene();


	//UFUNCTION()
	//bool IsAnyPreviousStepActive(const UPreplanStep* PreplanStep);

	UFUNCTION()
	void InitializePreplanSteps();
	
	UFUNCTION()
	void InitializePreplanAdvices();

	//UFUNCTION()
	//void InitializePreplanLinks();

	UFUNCTION()
	void InitializeCollectibles();

	//UFUNCTION()
	//void SetPreplanVisibility(UPreplanDataWidget* PreplanData, bool bIsVisible);

	UFUNCTION()
	void CreatePreplanStep(UPreplanDataWidget* PreplanDataWidget);

	UFUNCTION()
	void CreateCollectible(const Acollectible* Collectible);

public:
	UFUNCTION()
	void OnAdvicesVisibilityChanged(bool bNewVisibility);
	
	UFUNCTION(BlueprintCallable)
	void InitializePreplan();

	UFUNCTION(BlueprintCallable)
	void ActivatePreplanStep(FString PreplanID,int StepPart=1);

	UFUNCTION(BlueprintCallable)
	void ActivateCollectible(FString CollectibleID);

	UFUNCTION(BlueprintCallable)
	void SaveLoopData();

	UFUNCTION(BlueprintCallable)
	void LoadLoopData();

	UFUNCTION(BlueprintCallable)
	void ResetAllProgress();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsLoopNbInSaveZero();

	UFUNCTION(BlueprintCallable)
	void CheckAllPreplanStepsUnlocked();

	UFUNCTION()
	void CheckLoopCountAchievement();

	UFUNCTION(BlueprintCallable)
	void OnEnemyKilled();

	UFUNCTION()
	void CheckEnemyKillCountAchievement();
	
private:
	UPROPERTY()
	bool bHasUnlockedAllPreplanStepsAchievement = false;

	UPROPERTY()
	bool bHasUnlockedWildsAchievement = false;

	UPROPERTY()
	bool bHasUnlockedGetOffMeAchievement = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 TotalEnemyKillCount = 0;
};
