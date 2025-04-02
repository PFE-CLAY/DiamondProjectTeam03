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

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable, Category = "Wwise", meta = (AdvancedDisplay = "2"))
	void PlayEvent(UAkAudioEvent* Event, AActor* TargetActor, const FOnAkPostEventCallback& PostEventCallback);

	UFUNCTION(BlueprintCallable, Category="Wwise", meta = (AdvancedDisplay = "2"))
	void SetCategoryVolume(ESoundCategory Category, float Volume, int32 InterpolateTimeMs, AActor* TargetActor);
};