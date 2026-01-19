// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "DiamondProject/DiamondProjectPlayerController.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameFramework/GameUserSettings.h"
#include "GameSettingsSubsystem.generated.h"

UENUM(BlueprintType)
enum class EGraphicsQuality : uint8
{
	LOW,
	MEDIUM,
	HIGH,
	VERY_HIGH,
};

/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API UGameSettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAdvicesVisibilityChangedEvent, bool, NewVisibility);
	

	
public:

	
	UPROPERTY(BlueprintAssignable)
	FOnAdvicesVisibilityChangedEvent OnAdvicesVisibilityChangedDelegate;

	UPROPERTY(BlueprintReadWrite)
	bool bSubtitlesActivated = true;
	
	UPROPERTY(BlueprintReadWrite)
	EKeyboardLayout SavedKeyboardLayout = EKeyboardLayout::UNSPECIFIED;

	UPROPERTY(BlueprintReadWrite)
	float SubtitlesBackgroundOpacity = 0.5f;

	UPROPERTY(BlueprintReadWrite)
	float MouseSensitivity = 1.0f;
	
	UPROPERTY(BlueprintReadWrite)
	FString CrosshairColor = "White";

	UPROPERTY(BlueprintReadWrite)
	float CrosshairSize = 0.1f;
	
	UPROPERTY(BlueprintReadWrite)
	float DashUISize = 0.1f;

	UPROPERTY(BlueprintReadWrite)
	float MasterVolume = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	float MusicVolume = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	float SFXVolume = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	float VoiceVolume = 1.0f;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsOnboardingTutorialActivated = true;
	
private:
	
	UPROPERTY()
	int32 SelectedGraphicsQualityLevel = 2; // Default High
	
public:
	UFUNCTION(BlueprintCallable)
	bool IsPreplanInSceneVisible() const;
	UFUNCTION(BlueprintCallable)
	void SetPreplanInSceneVisibility(bool IsPreplanVisible);

	UFUNCTION(BlueprintCallable)
	void SaveSettings();

	UFUNCTION(BlueprintCallable)
	void LoadSettings();

	UFUNCTION(BlueprintCallable)
	void ResetSettings();
	
	UFUNCTION(BlueprintCallable)
	void ChangeGraphicsSettings(EGraphicsQuality QualityLevel);
	
	UFUNCTION(BlueprintCallable)
	int32 GetSelectedGraphicsQualityLevel() const { return SelectedGraphicsQualityLevel; }

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
