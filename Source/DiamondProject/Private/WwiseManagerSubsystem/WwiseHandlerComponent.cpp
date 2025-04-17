// Fill out your copyright notice in the Description page of Project Settings.

#include "AkGameplayTypes.h"
#include "WwiseManagerSubsystem/WwiseHandlerComponent.h"

// Sets default values for this component's properties
UWwiseHandlerComponent::UWwiseHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UWwiseHandlerComponent::BeginPlay()
{
	Super::BeginPlay();
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
			break;
	}
}

void UWwiseHandlerComponent::OnEndOfEventCallback_Implementation(UAkEventCallbackInfo* CallbackInfo)
{
	
}

void UWwiseHandlerComponent::OnMarkerCallback_Implementation(UAkMarkerCallbackInfo* CallbackInfo)
{
	
}

void UWwiseHandlerComponent::OnDurationCallback_Implementation(UAkDurationCallbackInfo* CallbackInfo)
{
	
}

void UWwiseHandlerComponent::OnStarvationCallback_Implementation(UAkEventCallbackInfo* CallbackInfo)
{
	
}

void UWwiseHandlerComponent::OnMusicPlayStartedCallback_Implementation(UAkEventCallbackInfo* CallbackInfo)
{
	
}

void UWwiseHandlerComponent::OnMidiEventCallback_Implementation(UAkMIDIEventCallbackInfo* CallbackInfo)
{
	
}
