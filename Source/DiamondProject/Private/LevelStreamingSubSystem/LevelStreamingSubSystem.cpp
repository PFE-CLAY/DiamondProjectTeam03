// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamingSubSystem/LevelStreamingSubSystem.h"

#include "Kismet/GameplayStatics.h"

ULevelStreaming* ULevelStreamingSubSystem::LoadLevelByName(ULevel* Level, FName LevelName, bool bMakeVisibleAfterLoad, bool bShouldBlockOnLoad)
{
	FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(this, LevelName, bMakeVisibleAfterLoad, bShouldBlockOnLoad, LatentInfo);

	LoadedLevelsCache.Add(LevelName, UGameplayStatics::GetStreamingLevel(Level, LevelName));
	return LoadedLevelsCache[LevelName];
}

void ULevelStreamingSubSystem::UnloadLevelByName(FName LevelName)
{
	FLatentActionInfo LatentInfo;
	
	if (!LoadedLevelsCache.Contains(LevelName)) {
		UE_LOG(LogTemp, Warning, TEXT("[LevelStreamingSubSystem::UnloadLevelByName] Level %s is not loaded or not tracked in cache."), *LevelName.ToString());
		return;
	}
	UGameplayStatics::UnloadStreamLevel(this, LevelName, LatentInfo, true);
	LoadedLevelsCache.Remove(LevelName);
}

ULevelStreaming* ULevelStreamingSubSystem::GetLoadedLevel(FName LevelName) const
{
	if (LoadedLevelsCache.Contains(LevelName)) {
		return LoadedLevelsCache[LevelName];
	}
	UE_LOG(LogTemp, Warning, TEXT("[LeveLStreamingSubSystem::GetLoadedLevel] Level [%s] is not loaded or not tracked in cache."), *LevelName.ToString());
	return nullptr;
}