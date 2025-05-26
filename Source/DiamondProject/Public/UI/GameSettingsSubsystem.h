// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameSettingsSubsystem.generated.h"

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
	float SubtitlesBackgroundOpacity = 0.5f;

	UPROPERTY(BlueprintReadWrite)
	float MouseSensitivity = 1.0f;
	
private:
	UPROPERTY(EditAnywhere)
	bool bIsPreplanInSceneVisible = false;
	
public:
	UFUNCTION(BlueprintCallable)
	bool IsPreplanInSceneVisible() const;
	UFUNCTION(BlueprintCallable)
	void SetPreplanInSceneVisibility(bool IsPreplanVisible);
};
