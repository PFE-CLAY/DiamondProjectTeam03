// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DiamondProject/DiamondProjectPlayerController.h"
#include "GameFramework/SaveGame.h"
#include "DiamondSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FGameSettingsSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	bool bSubtitlesActivated = true;

	UPROPERTY()
	float SubtitlesBackgroundOpacity = 0.5f;

	UPROPERTY()
	float MouseSensitivity = 1.0f;

	UPROPERTY()
	FString CrosshairColor = "White";

	UPROPERTY()
	float CrosshairSize = 0.1f;

	UPROPERTY()
	float DashUISize = 0.1f;

	UPROPERTY()
	float MasterVolume = 1.0f;

	UPROPERTY()
	float MusicVolume = 1.0f;

	UPROPERTY()
	float SFXVolume = 1.0f;

	UPROPERTY()
	float VoiceVolume = 1.0f;

	UPROPERTY()
	bool bIsPreplanInSceneVisible = false;
	
	UPROPERTY()
	EKeyboardLayout SavedKeyboardLayout = EKeyboardLayout::UNSPECIFIED;
	
	UPROPERTY()
	int32 SelectedGraphicsQualityLevel = 3;
};

USTRUCT(BlueprintType)
struct FPreplanStepSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	int32 FirstNbActivations = 0;

	UPROPERTY()
	int32 SndNbActivations = 0;

	UPROPERTY()
	bool bIsStepActive = false;

	UPROPERTY()
	bool bIsFirstStepVisible = false;

	UPROPERTY()
	bool bIsFirstAlreadySeen = false;

	UPROPERTY()
	bool bIsSndStepVisible = false;

	UPROPERTY()
	bool bIsSndAlreadySeen = false;
};

USTRUCT(BlueprintType)
struct FLoopSystemSaveData
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 LoopNb = 0;

	UPROPERTY()
	TMap<FString, FPreplanStepSaveData> PreplanSteps;

	UPROPERTY()
	TMap<FString, bool> Collectibles;

	UPROPERTY()
	bool bIsDreamLevel = false;

	UPROPERTY()
	bool IsDeadByTimer = false;
};

/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API UDiamondSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UDiamondSaveGame();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = SaveData)
	FGameSettingsSaveData SettingsData;

	UPROPERTY(VisibleAnywhere, Category = SaveData)
	FLoopSystemSaveData LoopData;
};
