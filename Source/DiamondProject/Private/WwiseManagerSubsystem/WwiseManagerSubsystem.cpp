// Fill out your copyright notice in the Description page of Project Settings.

#include "WwiseManagerSubsystem/WwiseManagerSubsystem.h"

#include "AkRtpc.h"
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
	if (Event && TargetActor) {
		EventCurrentlyPlayedByActor.Add(TargetActor, Event);
		UAkGameplayStatics::PostEvent(Event, TargetActor, 0, PostEventCallback);
		UE_LOG(LogTemp, Log, TEXT("Playing Wwise Event: %s"), *Event->GetName());
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Event or TargetActor is null!"));
	}
}

void UWwiseManagerSubsystem::SetRTPCValue(const UAkRtpc* RTPCValue, float Value, int32 InterpolationTimeMs, AActor* Actor)
{
	if (RTPCValue && Actor) {
		UAkGameplayStatics::SetRTPCValue(RTPCValue, Value, InterpolationTimeMs, Actor, FName());
		UE_LOG(LogTemp, Log, TEXT("Set RTPC Value: %s to %f"), *RTPCValue->GetName(), Value);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("RTPCValue or Actor is null!"));
	}
}

void UWwiseManagerSubsystem::SetCategoryVolume(ESoundCategory Category, float Volume, AActor* TargetActor, int32 InterpolateTimeMs)
{
	FString RTPCName;
	UAkRtpc* RTPC = nullptr;
	switch (Category) {
		case ESoundCategory::SFX: RTPCName = "Volume_SFX"; break;
		case ESoundCategory::Music: RTPCName = "Volume_Music"; break;
		case ESoundCategory::UI: RTPCName = "Volume_UI"; break;
		case ESoundCategory::Ambience: RTPCName = "Volume_Ambience"; break;
	}  
	
	if (!RTPCName.IsEmpty()) {
		UAkGameplayStatics::SetRTPCValue(nullptr, Volume, InterpolateTimeMs, TargetActor, FName(RTPCName));
		UE_LOG(LogTemp, Log, TEXT("Set volume for %s to %f"), *RTPCName, Volume);
	}
}