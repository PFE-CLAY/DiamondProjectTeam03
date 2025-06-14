﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "WwiseManagerSubsystem/WwiseHandlerComponent.h"
#include "AkGameplayTypes.h"

// Sets default values for this component's properties
UWwiseHandlerComponent::UWwiseHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CallbackMask |= 1 << static_cast<int32>(EAkCallbackType::EndOfEvent) | 1 << static_cast<int32>(EAkCallbackType::Marker) |
		1 << static_cast<int32>(EAkCallbackType::Duration) | 1 << static_cast<int32>(EAkCallbackType::Starvation) |
		1 << static_cast<int32>(EAkCallbackType::MusicPlayStarted) | 1 << static_cast<int32>(EAkCallbackType::MIDIEvent);

	UE_LOG(LogTemp, Log, TEXT("[UWwiseHandlerComponent] initialized with CallbackMask: %d"), CallbackMask);
}

// Called when the game starts
void UWwiseHandlerComponent::BeginPlay()
{
	Super::BeginPlay();
	CallbackMask |= 1 << static_cast<int32>(EAkCallbackType::EndOfEvent) | 1 << static_cast<int32>(EAkCallbackType::Marker) |
		1 << static_cast<int32>(EAkCallbackType::Duration) | 1 << static_cast<int32>(EAkCallbackType::Starvation) |
		1 << static_cast<int32>(EAkCallbackType::MusicPlayStarted) | 1 << static_cast<int32>(EAkCallbackType::MIDIEvent);
	UE_LOG(LogTemp, Log, TEXT("[UWwiseHandlerComponent] BeginPlay called on %s callback Mask is egal to : %d"), *GetOwner()->GetName(), CallbackMask);
}

void UWwiseHandlerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}


// Called every frame
void UWwiseHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWwiseHandlerComponent::HandleCallback(EAkCallbackType CallbackType, UAkCallbackInfo* CallbackInfo)
{
	switch (CallbackType) {
		case EAkCallbackType::EndOfEvent:
			OnEndOfEventCallback(CastChecked<UAkEventCallbackInfo>(CallbackInfo), this);
			break;
		case EAkCallbackType::Marker:
			OnMarkerCallback(CastChecked<UAkMarkerCallbackInfo>(CallbackInfo), this);
			break;
		case EAkCallbackType::Duration:
			OnDurationCallback(CastChecked<UAkDurationCallbackInfo>(CallbackInfo), this);
			break;
		case EAkCallbackType::Starvation:
			OnStarvationCallback(CastChecked<UAkEventCallbackInfo>(CallbackInfo), this);
			break;
		case EAkCallbackType::MusicPlayStarted:
			OnMusicPlayStartedCallback(CastChecked<UAkEventCallbackInfo>(CallbackInfo), this);
			break;
		case EAkCallbackType::MIDIEvent:
			OnMidiEventCallback(CastChecked<UAkMIDIEventCallbackInfo>(CallbackInfo), this);
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("[UWwiseHandlerComponent::HandleCallback] Unhandled Wwise callback type: %d on Actor: %s"), CallbackType, *GetOwner()->GetName());
			HandleCallbackDelegate.Broadcast(CallbackType, CallbackInfo);
			break;
	}
}

void UWwiseHandlerComponent::OnEndOfEventCallback(UAkEventCallbackInfo* CallbackInfo, UWwiseHandlerComponent* owner)
{
	UE_LOG(LogTemp, Log, TEXT("[UWwiseHandlerComponent::OnEndOfEvent] triggered in %s"), *GetOwner()->GetName());
	if (CallbackInfo == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("[UWwiseHandlerComponent::OnEndOfEvent] CallbackInfo is null!"));
		return;
	}
	EndOfEventCallbackDelegate.Broadcast(CallbackInfo, this);
}

void UWwiseHandlerComponent::OnMarkerCallback(UAkMarkerCallbackInfo* CallbackInfo, UWwiseHandlerComponent* owner)
{
	if (CallbackInfo == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("[UWwiseHandlerComponent::OnMarkerCallback] CallbackInfo is null!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[UWwiseHandlerComponent::OnMarkerCallback] Callback: Label = %s, Position = %u, Identifier = %u"),
			*CallbackInfo->Label, CallbackInfo->Position, CallbackInfo->Identifier);
	MarkerCallbackDelegate.Broadcast(CallbackInfo, this);
}

void UWwiseHandlerComponent::OnDurationCallback(UAkDurationCallbackInfo* CallbackInfo, UWwiseHandlerComponent* owner)
{
	if (CallbackInfo == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("[UWwiseHandlerComponent::OnDurationCallback] CallbackInfo is null!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[UWwiseHandlerComponent::OnDurationCallback] Callback: Duration = %f, EstimatedDuration = %f, AudioNodeID = %u, MediaID = %u"),
			CallbackInfo->Duration, CallbackInfo->EstimatedDuration, CallbackInfo->AudioNodeID, CallbackInfo->MediaID);
	DurationCallbackDelegate.Broadcast(CallbackInfo, this);
}

void UWwiseHandlerComponent::OnStarvationCallback(UAkEventCallbackInfo* CallbackInfo, UWwiseHandlerComponent* owner)
{
	if (CallbackInfo == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("[UWwiseHandlerComponent::OnStarvationCallback] CallbackInfo is null!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[UWwiseHandlerComponent::OnStarvationCallback] Callback: PlayingID = %u, EventID = %u"),
			CallbackInfo->PlayingID, CallbackInfo->EventID);
	StarvationCallbackDelegate.Broadcast(CallbackInfo, this);
}

void UWwiseHandlerComponent::OnMusicPlayStartedCallback(UAkEventCallbackInfo* CallbackInfo, UWwiseHandlerComponent* owner)
{
	if (CallbackInfo == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("[UWwiseHandlerComponent::OnMusicPlayStartedCallback] CallbackInfo is null!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[UWwiseHandlerComponent::OnMusicPlayStartedCallback] Callback: PlayingID = %u, EventID = %u"),
			CallbackInfo->PlayingID, CallbackInfo->EventID);
	MusicPlayStartedCallbackDelegate.Broadcast(CallbackInfo, this);
}

void UWwiseHandlerComponent::OnMidiEventCallback(UAkMIDIEventCallbackInfo* CallbackInfo, UWwiseHandlerComponent* ownerzh)
{
	if (CallbackInfo == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("[UWwiseHandlerComponent::OnMidiEventCallback] CallbackInfo is null!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[UWwiseHandlerComponent::OnMidiEventCallback] Callback: PlayingID = %u, EventID = %u"),
			CallbackInfo->PlayingID, CallbackInfo->EventID);
	MidiEventCallbackDelegate.Broadcast(CallbackInfo, this);
}

void UWwiseHandlerComponent::AddMaskToCallbackMask(EAkCallbackType CallbackType)
{
	UE_LOG(LogTemp, Log, TEXT("[UWwiseHandlerComponent] Added %s to Callback Mask on Actor %s"), *UEnum::GetValueAsString(CallbackType), *GetOwner()->GetName());
	CallbackMask |= 1 << static_cast<int32>(CallbackType);
	UE_LOG(LogTemp, Log, TEXT("[UWwiseHandlerComponent] Callback Mask is now: %d"), CallbackMask);
}

void UWwiseHandlerComponent::RemoveMaskFromCallbackMask(EAkCallbackType CallbackType)
{
	UE_LOG(LogTemp, Log, TEXT("[UWwiseHandlerComponent] Removed %s from Callback Mask on Actor %s"), *UEnum::GetValueAsString(CallbackType), *GetOwner()->GetName());
	CallbackMask &= ~(1 << static_cast<int32>(CallbackType));
	UE_LOG(LogTemp, Log, TEXT("[UWwiseHandlerComponent] Callback Mask is now: %d"), CallbackMask);
}


