// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelStreamingSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API ULevelStreamingSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TMap<FName, TObjectPtr<ULevelStreaming>> LoadedLevelsCache;

public:
	UFUNCTION(BlueprintCallable, Category = "LevelStreaming SubSystem", meta = (AdvancedDisplay = "1"))
	ULevelStreaming* LoadLevelByName(ULevel* Level, FName LevelName, bool bMakeVisibleAfterLoad = true, bool bShouldBlockOnLoad = true);

	UFUNCTION(BlueprintCallable, Category = "LevelStreaming SubSystem")
	void UnloadLevelByName(FName LevelName);

	UFUNCTION()
	ULevelStreaming* GetLoadedLevel(FName LevelName) const;
};
