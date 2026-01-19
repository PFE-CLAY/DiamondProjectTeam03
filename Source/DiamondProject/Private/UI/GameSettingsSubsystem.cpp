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

void UGameSettingsSubsystem::ChangeGraphicsSettings(int32 QualityLevel)
{
	// 1. Get the GameUserSettings pointer
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	if (!UserSettings) return;

	SelectedGraphicsQualityLevel = QualityLevel;
	
	// 2. Set individual scalability groups
	// QualityLevel: 0=Low, 1=Medium, 2=High, 3=Epic, 4=Cinematic
    
	// UserSettings->SetTextureQuality(QualityLevel);
	// UserSettings->SetShadowQuality(QualityLevel);
	// UserSettings->SetVisualEffectQuality(QualityLevel);
	// UserSettings->SetPostProcessingQuality(QualityLevel);
	// UserSettings->SetAntiAliasingQuality(QualityLevel);
	// UserSettings->SetFoliageQuality(QualityLevel);
	// UserSettings->SetShadingQuality(QualityLevel);
	// UserSettings->SetGlobalIlluminationQuality(QualityLevel);
	// UserSettings->SetReflectionQuality(QualityLevel);
	// UserSettings->SetViewDistanceQuality(QualityLevel);

	// Optional: Set the overall scalability level (sets all groups at once)
	UserSettings->SetOverallScalabilityLevel(SelectedGraphicsQualityLevel);

	// 3. Apply the settings to the engine (Actual visual change happens here)
	// passing 'false' prevents the engine from checking for command line overrides
	UserSettings->ApplySettings(false);

	// 4. Save settings to disk (GameUserSettings.ini)
	// ApplySettings() usually saves automatically, but you can force it here
	UserSettings->SaveSettings();
	
	SaveSettings();
}
