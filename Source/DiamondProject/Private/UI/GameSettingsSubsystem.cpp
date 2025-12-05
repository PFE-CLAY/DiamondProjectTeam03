// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameSettingsSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/DiamondSaveGame.h"

bool UGameSettingsSubsystem::IsPreplanInSceneVisible() const
{
	return bIsPreplanInSceneVisible;
}

void UGameSettingsSubsystem::SetPreplanInSceneVisibility(bool IsPreplanVisible)
{
	bIsPreplanInSceneVisible = IsPreplanVisible;
	OnAdvicesVisibilityChangedDelegate.Broadcast(bIsPreplanInSceneVisible);
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
		SaveGameInstance->SettingsData.bIsPreplanInSceneVisible = bIsPreplanInSceneVisible;
		SaveGameInstance->SettingsData.SavedKeyboardLayout = SavedKeyboardLayout;

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
		bIsPreplanInSceneVisible = SaveGameInstance->SettingsData.bIsPreplanInSceneVisible;
		SavedKeyboardLayout = SaveGameInstance->SettingsData.SavedKeyboardLayout;
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
	bIsPreplanInSceneVisible = DefaultData.bIsPreplanInSceneVisible;
	SavedKeyboardLayout = DefaultData.SavedKeyboardLayout;
	
	SaveSettings();
}
