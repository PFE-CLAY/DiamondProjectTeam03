// Fill out your copyright notice in the Description page of Project Settings.

#include "../Plugins/Wwise/Source/AkAudio/Classes/AkAudioEvent.h"
#include "WwiseManagerSubsystem/WwiseManagerSubsystem.h"

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