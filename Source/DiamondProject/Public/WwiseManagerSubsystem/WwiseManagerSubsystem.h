// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkGameplayStatics.h"
#include "WwiseManagerSubsystem.generated.h"


UENUM(BlueprintType)
enum class ESoundCategory : uint8
{
	SFX     UMETA(DisplayName = "SFX"),
	Music   UMETA(DisplayName = "Music"),
	UI      UMETA(DisplayName = "UI"),
	Ambience UMETA(DisplayName = "Ambience")
};

/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API UWwiseManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TMap<AActor*, const UAkAudioEvent*> EventCurrentlyPlayedByActor;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable, Category = "Wwise SubSystem")
	int32 PostEvent(UAkAudioEvent* Event, AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Wwise SubSystem")
	int32 PostEventWithoutActor(UAkAudioEvent* Event);
	
	UFUNCTION(BlueprintCallable, Category = "Wwise SubSystem")
	void SetRTPCValue(const UAkRtpc* RTPCValue, float Value, int32 InterpolationTimeMs, AActor* Actor);
	
	UFUNCTION(BlueprintCallable, Category="Wwise SubSystem", meta = (AdvancedDisplay = "3"))
	void SetCategoryVolume(const ESoundCategory Category, float Volume, AActor* TargetActor, int32 InterpolateTimeMs = 0);

	UFUNCTION(BlueprintCallable, Category = "Wwise SubSystem")
	void SetSwitch(const UAkSwitchValue* SwitchValue, AActor* Actor, FName SwitchGroup, FName SwitchState);

	UFUNCTION(BlueprintCallable, Category = "Wwise SubSystem")
	void SetState(const UAkStateValue* StateValue, FName stateGroup, FName state);
};