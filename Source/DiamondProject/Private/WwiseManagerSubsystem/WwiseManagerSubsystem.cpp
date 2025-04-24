// Fill out your copyright notice in the Description page of Project Settings.

#include "WwiseManagerSubsystem/WwiseManagerSubsystem.h"

#include "AkRtpc.h"
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

void UWwiseManagerSubsystem::PlayEvent(UAkAudioEvent* Event, AActor* TargetActor)
{
	FOnAkPostEventCallback Callback;
	UWwiseHandlerComponent* WwiseHandler = nullptr;
	
	if (Event && TargetActor) {
		EventCurrentlyPlayedByActor.Add(TargetActor, Event);
		WwiseHandler = TargetActor->FindComponentByClass<UWwiseHandlerComponent>();
		if (WwiseHandler) {
			Callback = FOnAkPostEventCallback();
			Callback.BindUFunction(WwiseHandler, "HandleCallback");
		} else {
			UE_LOG(LogTemp, Error, TEXT("WwiseHandlerComponent not found on TargetActor!"));
		}
		UAkGameplayStatics::PostEvent(Event, TargetActor, WwiseHandler->CallbackMask, Callback);
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

void UWwiseManagerSubsystem::SetSwitch(const UAkSwitchValue* SwitchValue, AActor* Actor, FName SwitchGroup, FName SwitchState)
{
	UE_LOG(LogTemp, Log, TEXT("Setting Switch State: %s to %s from actor: %s"), *SwitchGroup.ToString(), *SwitchState.ToString(), *Actor->GetName());
	UAkGameplayStatics::SetSwitch(SwitchValue, Actor, SwitchGroup, SwitchState);
}

void UWwiseManagerSubsystem::SetState(const UAkStateValue* StateValue, FName stateGroup, FName state)
{
	UE_LOG(LogTemp, Log, TEXT("Setting State: %s to %s"), *stateGroup.ToString(), *state.ToString());
	UAkGameplayStatics::SetState(StateValue, stateGroup, state);
}
