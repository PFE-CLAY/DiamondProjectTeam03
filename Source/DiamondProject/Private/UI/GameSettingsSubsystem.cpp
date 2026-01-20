// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameSettingsSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/DiamondSaveGame.h"

bool UGameSettingsSubsystem::IsPreplanInSceneVisible() const
{
	return bIsOnboardingTutorialActivated;
}

void UGameSettingsSubsystem::SetPreplanInSceneVisibility(bool IsPreplanVisible)
{
	bIsOnboardingTutorialActivated = IsPreplanVisible;
	OnAdvicesVisibilityChangedDelegate.Broadcast(bIsOnboardingTutorialActivated);
}

void UGameSettingsSubsystem::InitGraphicSettings()
{
	UserSettingsPtr = TStrongObjectPtr<UGameUserSettings>(UGameUserSettings::GetGameUserSettings());
}

void UGameSettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadSettings();
}

void UGameSettingsSubsystem::SaveSettings()
{
	UDiamondSaveGame* SaveGameInstance = Cast<UDiamondSaveGame>(UGameplayStatics::LoadGameFromSlot("DiamondSaveSlot", 0));
	if (!SaveGameInstance)
	{
		SaveGameInstance = Cast<UDiamondSaveGame>(UGameplayStatics::CreateSaveGameObject(UDiamondSaveGame::StaticClass()));
	}

	if (SaveGameInstance)
	{
		SaveGameInstance->SettingsData.bSubtitlesActivated = bSubtitlesActivated;
		SaveGameInstance->SettingsData.SubtitlesBackgroundOpacity = SubtitlesBackgroundOpacity;
		SaveGameInstance->SettingsData.MouseSensitivity = MouseSensitivity;
		SaveGameInstance->SettingsData.CrosshairColor = CrosshairColor;
		SaveGameInstance->SettingsData.CrosshairSize = CrosshairSize;
		SaveGameInstance->SettingsData.DashUISize = DashUISize;
		SaveGameInstance->SettingsData.MasterVolume = MasterVolume;
		SaveGameInstance->SettingsData.MusicVolume = MusicVolume;
		SaveGameInstance->SettingsData.SFXVolume = SFXVolume;
		SaveGameInstance->SettingsData.VoiceVolume = VoiceVolume;
		SaveGameInstance->SettingsData.bIsPreplanInSceneVisible = bIsOnboardingTutorialActivated;
		SaveGameInstance->SettingsData.SavedKeyboardLayout = SavedKeyboardLayout;
		SaveGameInstance->SettingsData.SelectedGraphicsQualityLevel = SelectedGraphicsQualityLevel;

		UGameplayStatics::SaveGameToSlot(SaveGameInstance, "DiamondSaveSlot", 0);
	}
}

void UGameSettingsSubsystem::LoadSettings()
{
	UDiamondSaveGame* SaveGameInstance = Cast<UDiamondSaveGame>(UGameplayStatics::LoadGameFromSlot("DiamondSaveSlot", 0));
	if (SaveGameInstance)
	{
		bSubtitlesActivated = SaveGameInstance->SettingsData.bSubtitlesActivated;
		SubtitlesBackgroundOpacity = SaveGameInstance->SettingsData.SubtitlesBackgroundOpacity;
		MouseSensitivity = SaveGameInstance->SettingsData.MouseSensitivity;
		CrosshairColor = SaveGameInstance->SettingsData.CrosshairColor;
		CrosshairSize = SaveGameInstance->SettingsData.CrosshairSize;
		DashUISize = SaveGameInstance->SettingsData.DashUISize;
		MasterVolume = SaveGameInstance->SettingsData.MasterVolume;
		MusicVolume = SaveGameInstance->SettingsData.MusicVolume;
		SFXVolume = SaveGameInstance->SettingsData.SFXVolume;
		VoiceVolume = SaveGameInstance->SettingsData.VoiceVolume;
		bIsOnboardingTutorialActivated = SaveGameInstance->SettingsData.bIsPreplanInSceneVisible;
		SavedKeyboardLayout = SaveGameInstance->SettingsData.SavedKeyboardLayout;
		SelectedGraphicsQualityLevel = SaveGameInstance->SettingsData.SelectedGraphicsQualityLevel;
	}
}

void UGameSettingsSubsystem::ResetSettings()
{
	FGameSettingsSaveData DefaultData;
	bSubtitlesActivated = DefaultData.bSubtitlesActivated;
	SubtitlesBackgroundOpacity = DefaultData.SubtitlesBackgroundOpacity;
	MouseSensitivity = DefaultData.MouseSensitivity;
	CrosshairColor = DefaultData.CrosshairColor;
	CrosshairSize = DefaultData.CrosshairSize;
	DashUISize = DefaultData.DashUISize;
	MasterVolume = DefaultData.MasterVolume;
	MusicVolume = DefaultData.MusicVolume;
	SFXVolume = DefaultData.SFXVolume;
	VoiceVolume = DefaultData.VoiceVolume;
	bIsOnboardingTutorialActivated = DefaultData.bIsPreplanInSceneVisible;
	SavedKeyboardLayout = DefaultData.SavedKeyboardLayout;
	SelectedGraphicsQualityLevel = DefaultData.SelectedGraphicsQualityLevel;
	
	SaveSettings();
}

void UGameSettingsSubsystem::ChangeGraphicsSettings(EGraphicsSettingType type ,EGraphicsQuality QualityLevel)
{
	int32 castedValue = static_cast<int32>(QualityLevel);
	
	switch (type)
	{
		case EGraphicsSettingType::TEXTURE_QUALITY:
			UserSettingsPtr->SetTextureQuality(castedValue);
			break;
		case EGraphicsSettingType::SHADOW_QUALITY:
			UserSettingsPtr->SetShadowQuality(castedValue);
			break;
		case EGraphicsSettingType::VISUAL_EFFECT_QUALITY:
			UserSettingsPtr->SetVisualEffectQuality(castedValue);
			break;
		case EGraphicsSettingType::POST_PROCESSING_QUALITY:
			UserSettingsPtr->SetPostProcessingQuality(castedValue);
			break;
		case EGraphicsSettingType::ANTI_ALIASING_QUALITY:
			UserSettingsPtr->SetAntiAliasingQuality(castedValue);
			break;
		case EGraphicsSettingType::SHADING_QUALITY:
			UserSettingsPtr->SetShadingQuality(castedValue);
			break;
		case EGraphicsSettingType::GLOBAL_ILLUMINATION_QUALITY:
			UserSettingsPtr->SetGlobalIlluminationQuality(castedValue);
			break;
		case EGraphicsSettingType::REFLECTION_QUALITY:
			UserSettingsPtr->SetReflectionQuality(castedValue);
			break;
	}
	
	UserSettingsPtr->SetOverallScalabilityLevel(castedValue);
}

void UGameSettingsSubsystem::ApplyGraphicSettings()
{
	UserSettingsPtr->ValidateSettings();
	
	if (UserSettingsPtr.IsValid())
	{
		UserSettingsPtr->ApplySettings(false);
	}
}
