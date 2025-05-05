// Fill out your copyright notice in the Description page of Project Settings.

#include "WwiseManagerSubsystem/WwiseHandlerComponent.h"
#include "AkGameplayTypes.h"

// Sets default values for this component's properties
UWwiseHandlerComponent::UWwiseHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CallbackMask |= 1 << static_cast<int32>(EAkCallbackType::EndOfEvent) | 1 << static_cast<int32>(EAkCallbackType::Marker) |
		1 << static_cast<int32>(EAkCallbackType::Duration) | 1 << static_cast<int32>(EAkCallbackType::Starvation) |
		1 << static_cast<int32>(EAkCallbackType::MusicPlayStarted) | 1 << static_cast<int32>(EAkCallbackType::MIDIEvent);

	UE_LOG(LogTemp, Log, TEXT("WwiseHandlerComponent initialized with CallbackMask: %d"), CallbackMask);
}

// Called when the game starts
void UWwiseHandlerComponent::BeginPlay()
{
	Super::BeginPlay();
	CallbackMask |= 1 << static_cast<int32>(EAkCallbackType::EndOfEvent) | 1 << static_cast<int32>(EAkCallbackType::Marker) |
		1 << static_cast<int32>(EAkCallbackType::Duration) | 1 << static_cast<int32>(EAkCallbackType::Starvation) |
		1 << static_cast<int32>(EAkCallbackType::MusicPlayStarted) | 1 << static_cast<int32>(EAkCallbackType::MIDIEvent);
	UE_LOG(LogTemp, Log, TEXT("WwiseHandlerComponent BeginPlay called on %s callback Mask is egal to : %d"), *GetOwner()->GetName(), CallbackMask);
}

// Called every frame
void UWwiseHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UWwiseHandlerComponent::HandleCallback_Implementation(EAkCallbackType CallbackType, UAkCallbackInfo* CallbackInfo)
{
	switch (CallbackType) {
		case EAkCallbackType::EndOfEvent:
			OnEndOfEventCallback(CastChecked<UAkEventCallbackInfo>(CallbackInfo));
			break;
		case EAkCallbackType::Marker:
			OnMarkerCallback(CastChecked<UAkMarkerCallbackInfo>(CallbackInfo));
			break;
		case EAkCallbackType::Duration:
			OnDurationCallback(CastChecked<UAkDurationCallbackInfo>(CallbackInfo));
			break;
		case EAkCallbackType::Starvation:
			OnStarvationCallback(CastChecked<UAkEventCallbackInfo>(CallbackInfo));
			break;
		case EAkCallbackType::MusicPlayStarted:
			OnMusicPlayStartedCallback(CastChecked<UAkEventCallbackInfo>(CallbackInfo));
			break;
		case EAkCallbackType::MIDIEvent:
			OnMidiEventCallback(CastChecked<UAkMIDIEventCallbackInfo>(CallbackInfo));
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Unhandled Wwise callback type: %d"), CallbackType);
			HandleCallbackDelegate.Broadcast(CallbackType, CallbackInfo);
			break;
	}
}

void UWwiseHandlerComponent::OnEndOfEventCallback_Implementation(UAkEventCallbackInfo* CallbackInfo)
{
	UE_LOG(LogTemp, Log, TEXT("[Wwise] EndOfEvent triggered in %s"), *GetOwner()->GetName());
	if (CallbackInfo == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("[OnEndOfEvent] CallbackInfo is null!"));
		return;
	}
	EndOfEventCallbackDelegate.Broadcast(CallbackInfo);
}

void UWwiseHandlerComponent::OnMarkerCallback_Implementation(UAkMarkerCallbackInfo* CallbackInfo)
{
	if (CallbackInfo == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("[OnMarkerCallback] CallbackInfo is null!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[Wwise] Marker Callback: Label = %s, Position = %u, Identifier = %u"),
			*CallbackInfo->Label, CallbackInfo->Position, CallbackInfo->Identifier);
	MarkerCallbackDelegate.Broadcast(CallbackInfo);
}

void UWwiseHandlerComponent::OnDurationCallback_Implementation(UAkDurationCallbackInfo* CallbackInfo)
{
	if (CallbackInfo == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("[OnDurationCallback] CallbackInfo is null!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[Wwise] Duration Callback: Duration = %f, EstimatedDuration = %f, AudioNodeID = %u, MediaID = %u"),
			CallbackInfo->Duration, CallbackInfo->EstimatedDuration, CallbackInfo->AudioNodeID, CallbackInfo->MediaID);
	DurationCallbackDelegate.Broadcast(CallbackInfo);
}

void UWwiseHandlerComponent::OnStarvationCallback_Implementation(UAkEventCallbackInfo* CallbackInfo)
{
	if (CallbackInfo == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("[OnStarvationCallback] CallbackInfo is null!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[Wwise] Starvation Callback: PlayingID = %u, EventID = %u"),
			CallbackInfo->PlayingID, CallbackInfo->EventID);
	StarvationCallbackDelegate.Broadcast(CallbackInfo);
}

void UWwiseHandlerComponent::OnMusicPlayStartedCallback_Implementation(UAkEventCallbackInfo* CallbackInfo)
{
	if (CallbackInfo == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("[OnMusicPlayStartedCallback] CallbackInfo is null!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[Wwise] Music Play Started Callback: PlayingID = %u, EventID = %u"),
			CallbackInfo->PlayingID, CallbackInfo->EventID);
	MusicPlayStartedCallbackDelegate.Broadcast(CallbackInfo);
}

void UWwiseHandlerComponent::OnMidiEventCallback_Implementation(UAkMIDIEventCallbackInfo* CallbackInfo)
{
	if (CallbackInfo == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("[OnMidiEventCallback."
							  ""
		 ""
   ""
   ""
   ""
   ""
   ".3] CallbackInfo is null!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[Wwise] MIDI Event Callback: PlayingID = %u, EventID = %u"),
			CallbackInfo->PlayingID, CallbackInfo->EventID);
	MidiEventCallbackDelegate.Broadcast(CallbackInfo);
}

void UWwiseHandlerComponent::AddMaskToCallbackMask(EAkCallbackType CallbackType)
{
	UE_LOG(LogTemp, Log, TEXT("WwiseHandlerComponent Added %s to Callback Mask on Actor %s"), *UEnum::GetValueAsString(CallbackType), *GetOwner()->GetName());
	CallbackMask |= 1 << static_cast<int32>(CallbackType);
	UE_LOG(LogTemp, Log, TEXT("WwiseHandlerComponent Callback Mask is now: %d"), CallbackMask);
}

void UWwiseHandlerComponent::RemoveMaskFromCallbackMask(EAkCallbackType CallbackType)
{
	UE_LOG(LogTemp, Log, TEXT("WwiseHandlerComponent Removed %s from Callback Mask on Actor %s"), *UEnum::GetValueAsString(CallbackType), *GetOwner()->GetName());
	CallbackMask &= ~(1 << static_cast<int32>(CallbackType));
	UE_LOG(LogTemp, Log, TEXT("WwiseHandlerComponent Callback Mask is now: %d"), CallbackMask);
}


