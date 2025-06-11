// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SubtitleSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName = "Subtitle Settings"))
class DIAMONDPROJECT_API USubtitleSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category = "Subtitle DataTable Settings", meta = (AllowedClasses="/Script/Engine.DataTable"))
	FSoftObjectPath CommentatorDataTable;
	
	UPROPERTY(Config, EditAnywhere, Category = "Subtitle DataTable Settings", meta = (AllowedClasses="/Script/Engine.DataTable"))
	FSoftObjectPath AlliesDataTable;
};