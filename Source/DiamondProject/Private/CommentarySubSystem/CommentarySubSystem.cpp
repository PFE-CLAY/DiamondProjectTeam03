// Fill out your copyright notice in the Description page of Project Settings.

#include "CommentarySubSystem/CommentarySubSystem.h"

#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include "AkAudioEvent.h"
#include "AkGameplayStatics.h"
#include "WwiseManagerSubsystem/WwiseHandlerComponent.h"
#include "WwiseManagerSubsystem/WwiseManagerSubsystem.h"

void UCommentarySubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("[UCommentarySubSystem] Initialized"));
}

void UCommentarySubSystem::Deinitialize()
{
	Super::Deinitialize();
	UE_LOG(LogTemp, Log, TEXT("[UCommentarySubSystem] Deinitialized"));
}

void UCommentarySubSystem::PlayCommentaryEvent(class UAkAudioEvent* Event, const int priority)
{
	PlayCommentaryEvent(Event, CommentaryActor, priority);
}

void UCommentarySubSystem::PlayCommentaryEvent(class UAkAudioEvent* Event, AActor* TargetActor, const int priority)
{
	UWwiseHandlerComponent* WwiseHandler = nullptr;

	UE_LOG(LogTemp, Log, TEXT("[UCommentarySubSystem::PlayCommentaryEvent] Event Path: %s Event: %s, TargetActor: %s, Priority: %d"), *Event->GetPathName(), *Event->GetName(), *TargetActor->GetName(), priority);
	if (Event && TargetActor) {
		if (!bIsPlaying || (CurrentEventPlayedWithPriority.Key != Event && CurrentEventPlayedWithPriority.Value >= priority)) {
			WwiseHandler = TargetActor->FindComponentByClass<UWwiseHandlerComponent>();
			if (WwiseHandler) {
				if (bIsPlaying && CurrentEventPlayedWithPriority.Value >= priority) {
					UAkGameplayStatics::StopActor(TargetActor);
					WwiseHandler->EndOfEventCallbackDelegate.AddDynamic(this, &UCommentarySubSystem::OnInterruption);
					OnCommentaryEventInterruptDelegate.BindLambda([this, TargetActor, Event, WwiseHandler]() {
						WwiseHandler->EndOfEventCallbackDelegate.AddDynamic(this, &UCommentarySubSystem::OnEndEvent);
						this->PlayingID = GetGameInstance()->GetSubsystem<UWwiseManagerSubsystem>()->PostEvent(Event, TargetActor);
						OnCommentaryEventInterruptDelegate.Unbind();
					});
					PlayingID = GetGameInstance()->GetSubsystem<UWwiseManagerSubsystem>()->PostEvent(InterruptEvent, TargetActor);
				} else {
					WwiseHandler->EndOfEventCallbackDelegate.AddDynamic(this, &UCommentarySubSystem::OnEndEvent);
					PlayingID = GetGameInstance()->GetSubsystem<UWwiseManagerSubsystem>()->PostEvent(Event, TargetActor);
					bIsPlaying = true;
				}
				CurrentEventPlayedWithPriority = {Event, priority};
			} else {
				UE_LOG(LogTemp, Error, TEXT("[UCommentarySubSystem::PlayCommentaryEvent] WwiseHandlerComponent not found on TargetActor!"));
			}
		} else {
			UE_LOG(LogTemp, Log, TEXT("[UCommentarySubSystem::PlayCommentaryEvent] Event already playing or priority too low!"));
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("[UCommentarySubSystem::PlayCommentaryEvent] Event or TargetActor is null!"));
	}
}

void UCommentarySubSystem::RegisterActor(AActor* TargetActor, UAkAudioEvent* CommentaryInterruptEvent)
{
	CommentaryActor = TargetActor;
	InterruptEvent = CommentaryInterruptEvent;
}


void UCommentarySubSystem::OnEndEvent(UAkEventCallbackInfo* CallbackInfo, UWwiseHandlerComponent* owner)
{
	bIsPlaying = false;
	CurrentEventPlayedWithPriority = {nullptr, -1};
	owner->EndOfEventCallbackDelegate.RemoveDynamic(this, &UCommentarySubSystem::OnEndEvent);
}

void UCommentarySubSystem::OnInterruption(UAkEventCallbackInfo* CallbackInfo, UWwiseHandlerComponent* owner)
{
	owner->EndOfEventCallbackDelegate.RemoveDynamic(this, &UCommentarySubSystem::OnInterruption);
	OnCommentaryEventInterruptDelegate.Execute();
}