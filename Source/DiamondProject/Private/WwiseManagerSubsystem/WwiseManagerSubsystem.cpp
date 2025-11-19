// Fill out your copyright notice in the Description page of Project Settings.

#include "WwiseManagerSubsystem/WwiseManagerSubsystem.h"

#include "AkRtpc.h"
#include "AkStateValue.h"
#include "AkGroupValue.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkAudioEvent.h"
#include "WwiseManagerSubsystem/WwiseHandlerComponent.h"

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

int32 UWwiseManagerSubsystem::PostEvent(UAkAudioEvent* Event, AActor* TargetActor)
{
	AkPlayingID PlayingID = AK_INVALID_PLAYING_ID;
	UWwiseHandlerComponent* WwiseHandler = nullptr;
	
	if (Event && TargetActor) {
		EventCurrentlyPlayedByActor.Add(TargetActor, Event);
		WwiseHandler = TargetActor->FindComponentByClass<UWwiseHandlerComponent>();
		if (WwiseHandler) {
			WwiseHandler->Callback = FOnAkPostEventCallback();
			WwiseHandler->Callback.BindDynamic(WwiseHandler, &UWwiseHandlerComponent::HandleCallback);
		} else {
			UE_LOG(LogTemp, Error, TEXT("[UWwiseManagerSubsystem::PostEvent] WwiseHandlerComponent not found on TargetActor: %s!"), *TargetActor->GetName());
		}
		PlayingID = UAkGameplayStatics::PostEvent(Event, TargetActor, WwiseHandler->CallbackMask, WwiseHandler->Callback);
		WwiseHandler->LastPlayedID = PlayingID;
		UE_LOG(LogTemp, Log, TEXT("[UWwiseManagerSubsystem::PostEvent] Playing Wwise Event: %s ID: %d"), *Event->GetName(), PlayingID);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("[UWwiseManagerSubsystem::PostEvent] Event or TargetActor is null!"));
	}
	if (PlayingID == AK_INVALID_PLAYING_ID) {
		UE_LOG(LogTemp, Error, TEXT("[UWwiseManagerSubsystem::PostEvent] Failed to post event!"));
	}
	return PlayingID;
}

int32 UWwiseManagerSubsystem::PostEventWithoutActor(UAkAudioEvent* Event)
{
	AkPlayingID PlayingID = AK_INVALID_PLAYING_ID;
	
	if (Event) {
		PlayingID = UAkGameplayStatics::PostEvent(Event, nullptr, 0, FOnAkPostEventCallback());
	}
	return PlayingID;
}

void UWwiseManagerSubsystem::SetRTPCValue(const UAkRtpc* RTPCValue, float Value, int32 InterpolationTimeMs, AActor* Actor)
{
	if (RTPCValue && Actor) {
		UAkGameplayStatics::SetRTPCValue(RTPCValue, Value, InterpolationTimeMs, Actor);
		UE_LOG(LogTemp, Log, TEXT("[UWwiseManagerSubsystem::SetRTPC] Set RTPC Value: %s to %f"), *RTPCValue->GetName(), Value);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("[UWwiseManagerSubsystem::SetRTPC] RTPCValue or Actor is null!"));
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
		UAkGameplayStatics::SetRTPCValue(nullptr, Volume, InterpolateTimeMs, TargetActor);
		UE_LOG(LogTemp, Log, TEXT("[UWwiseManagerSubsystem::SetCategoryVolume] Set volume for %s to %f"), *RTPCName, Volume);
	}
}

void UWwiseManagerSubsystem::SetSwitch(const UAkSwitchValue* SwitchValue, AActor* Actor)
{
	if (!SwitchValue) {
		UE_LOG(LogTemp, Warning, TEXT("[UWwiseManagerSubsystem::SetSwitch] SwitchValue is null!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[UWwiseManagerSubsystem::SetSwitch] Setting Switch State from actor: %s"), *Actor->GetName());
	UAkGameplayStatics::SetSwitch(SwitchValue, Actor);
}

void UWwiseManagerSubsystem::SetState(UAkStateValue* StateValue)
{
	if (!StateValue) {
		UE_LOG(LogTemp, Warning, TEXT("[UWwiseManagerSubsystem::SetState] StateValue is null!"));
		return;
	}
	#if UE_EDITOR
	UE_LOG(LogTemp, Log, TEXT("[UWwiseManagerSubsystem::SetState] Setting State: %s"), *StateValue->GetInfoMutable()->WwiseName.ToString());
	#else
	UE_LOG(LogTemp, Log, TEXT("[UWwiseManagerSubsystem::SetState] Setting State"));
	#endif
	
	UAkGameplayStatics::SetState(StateValue);
}
