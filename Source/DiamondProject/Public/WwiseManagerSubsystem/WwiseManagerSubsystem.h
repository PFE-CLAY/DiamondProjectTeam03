// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkGameplayStatics.h"
#include "WwiseManagerSubsystem.generated.h"
/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API UWwiseManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable, Category = "Wwise")
	void PlayEvent(UAkAudioEvent* Event, AActor* TargetActor, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/AkAudio.EAkCallbackType")) const FOnAkPostEventCallback& PostEventCallback);
};
