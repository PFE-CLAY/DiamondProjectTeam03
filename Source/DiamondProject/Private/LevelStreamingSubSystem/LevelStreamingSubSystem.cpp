// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamingSubSystem/LevelStreamingSubSystem.h"

#include "Kismet/GameplayStatics.h"

ULevelStreaming* ULevelStreamingSubSystem::LoadLevelByName(FName LevelName, bool bMakeVisibleAfterLoad, bool bShouldBlockOnLoad)
{
	FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(this, LevelName, true, true, LatentInfo);

	LoadedLevelsCache.Add(LevelName, UGameplayStatics::GetStreamingLevel(this, LevelName));
	return LoadedLevelsCache[LevelName];
}

void ULevelStreamingSubSystem::UnloadLevelByName(FName LevelName)
{
	FLatentActionInfo LatentInfo;
	
	if (!LoadedLevelsCache.Contains(LevelName)) {
		UE_LOG(LogTemp, Warning, TEXT("Level %s is not loaded or not tracked in cache."), *LevelName.ToString());
		return;
	}
	UGameplayStatics::UnloadStreamLevel(this, LevelName, LatentInfo, true);
	LoadedLevelsCache.Remove(LevelName);
}