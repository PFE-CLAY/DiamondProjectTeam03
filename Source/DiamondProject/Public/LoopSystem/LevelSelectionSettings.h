// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "LevelSelectionSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "Level Selection Settings"))
class DIAMONDPROJECT_API ULevelSelectionSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	ULevelSelectionSettings();
	
	UPROPERTY(Config, EditAnywhere, Category="Levels")
	TSoftObjectPtr<UWorld> MainLevel;

	UPROPERTY(Config, EditAnywhere, Category="Levels")
	TSoftObjectPtr<UWorld> DreamLevel;
};
