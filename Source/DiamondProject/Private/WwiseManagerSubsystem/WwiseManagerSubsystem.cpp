// Fill out your copyright notice in the Description page of Project Settings.

#include "WwiseManagerSubsystem/WwiseManagerSubsystem.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkAudioEvent.h"

void UWwiseManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("WwiseAudioManagerSubsystem Initialized"));
}

void UWwiseManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	UE_LOG(LogTemp, Log, TEXT("WwiseAudioManagerSubsystem Deinitialized"));
}

void UWwiseManagerSubsystem::PlayEvent(UAkAudioEvent* Event, AActor* TargetActor, const FOnAkPostEventCallback& PostEventCallback)
{
	if (Event && TargetActor)
	{
		UAkGameplayStatics::PostEvent(Event, TargetActor, 0, PostEventCallback);
		UE_LOG(LogTemp, Log, TEXT("Playing Wwise Event: %s"), *Event->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Event or TargetActor is null!"));
	}
}

void UWwiseManagerSubsystem::SetCategoryVolume(ESoundCategory Category, float Volume, int32 InterpolateTimeMs, AActor* TargetActor)
{
	FString RTPCName;
	UAkRtpc* RTPC = nullptr;
	switch (Category)
	{
	case ESoundCategory::SFX: RTPCName = "Volume_SFX"; break;
	case ESoundCategory::Music: RTPCName = "Volume_Music"; break;
	case ESoundCategory::UI: RTPCName = "Volume_UI"; break;
	case ESoundCategory::Ambience: RTPCName = "Volume_Ambience"; break;
	}
	RTPC = LoadObject<UAkRtpc>(nullptr, *RTPCName);
	
	if (!RTPCName.IsEmpty())
	{
		UAkGameplayStatics::SetRTPCValue(RTPC, Volume, InterpolateTimeMs, TargetActor, FName(RTPCName));
		UAkGameplayStatics::SetSwitch()
		UE_LOG(LogTemp, Log, TEXT("Set volume for %s to %f"), *RTPCName, Volume);
	}
}