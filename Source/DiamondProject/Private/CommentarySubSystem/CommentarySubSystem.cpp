// Fill out your copyright notice in the Description page of Project Settings.

#include "CommentarySubSystem/CommentarySubSystem.h"

#include "AkAudioEvent.h"
#include "AkGameplayStatics.h"

void UCommentarySubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("WwiseAudioManagerSubsystem Initialized"));
}

void UCommentarySubSystem::Deinitialize()
{
	Super::Deinitialize();
	UE_LOG(LogTemp, Log, TEXT("WwiseAudioManagerSubsystem Deinitialized"));
}

void UCommentarySubSystem::PlayCommentaryEvent(class UAkAudioEvent* Event, AActor* TargetActor, const int priority, const FOnAkPostEventCallback& PostEventCallback)
{
	if (Event && TargetActor) {
		if (CurrentEvenPlayedWithPriority.Key != Event && CurrentEvenPlayedWithPriority.Value <= priority) {
			CurrentEvenPlayedWithPriority = {Event, priority};
			UAkGameplayStatics::PostEvent(Event, TargetActor, priority, PostEventCallback);
			UE_LOG(LogTemp, Log, TEXT("Playing Commentary Event: %s"), *Event->GetName());
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Event or TargetActor is null!"));
	}
}
