// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameSettingsSubsystem.h"

bool UGameSettingsSubsystem::IsPreplanInSceneVisible() const
{
	return bIsPreplanInSceneVisible;
}

void UGameSettingsSubsystem::SetPreplanInSceneVisibility(bool IsPreplanVisible)
{
	bIsPreplanInSceneVisible = IsPreplanVisible;
	OnAdvicesVisibilityChangedDelegate.Broadcast(bIsPreplanInSceneVisible);
}
