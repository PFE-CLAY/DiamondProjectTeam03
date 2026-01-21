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
	CINEMATIC
};

UENUM(BlueprintType)
enum class EGraphicsSettingType : uint8
{
	TEXTURE_QUALITY,
	SHADOW_QUALITY,
	VISUAL_EFFECT_QUALITY,
	POST_PROCESSING_QUALITY,
	ANTI_ALIASING_QUALITY,
	SHADING_QUALITY,
	GLOBAL_ILLUMINATION_QUALITY,
	REFLECTION_QUALITY,
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
	TObjectPtr<UGameUserSettings> UserSettingsPtr;
	
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
	void ChangeGraphicsSettings(EGraphicsSettingType Type, EGraphicsQuality QualityLevel);
	
	UFUNCTION(BlueprintCallable)
	void InitGraphicSettings();

	UFUNCTION(BlueprintCallable)
	void ApplyGraphicSettings();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
#pragma region GraphicsSettingsGetters
	
	UFUNCTION(BlueprintCallable)
	int32 GetTextureQuality() const { return UserSettingsPtr ? UserSettingsPtr->GetTextureQuality() : 0; }

	UFUNCTION(BlueprintCallable)
	int32 GetShadowQuality() const { return UserSettingsPtr ? UserSettingsPtr->GetShadowQuality() : 0; }

	UFUNCTION(BlueprintCallable)
	int32 GetVisualEffectsQuality() const { return UserSettingsPtr ? UserSettingsPtr->GetVisualEffectQuality() : 0; }

	UFUNCTION(BlueprintCallable)
	int32 GetPostProcessingQuality() const { return UserSettingsPtr ? UserSettingsPtr->GetPostProcessingQuality() : 0; }

	UFUNCTION(BlueprintCallable)
	int32 GetAntiAliasingQuality() const { return UserSettingsPtr ? UserSettingsPtr->GetAntiAliasingQuality() : 0; }

	UFUNCTION(BlueprintCallable)
	int32 GetShadingQuality() const { return UserSettingsPtr ? UserSettingsPtr->GetShadingQuality() : 0; }

	UFUNCTION(BlueprintCallable)
	int32 GetGlobalIlluminationQuality() const { return UserSettingsPtr ? UserSettingsPtr->GetGlobalIlluminationQuality() : 0; }

	UFUNCTION(BlueprintCallable)
	int32 GetReflectionQuality() const { return UserSettingsPtr ? UserSettingsPtr->GetReflectionQuality() : 0; }
#pragma endregion
};
