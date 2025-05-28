// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AkAudioEvent.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommentarySubSystem.generated.h"

class UWwiseHandlerComponent;
/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API UCommentarySubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	TPair<UAkAudioEvent*, int> CurrentEventPlayedWithPriority = {nullptr, -1};

	UPROPERTY()
	TObjectPtr<AActor> CommentaryActor;
	
	UPROPERTY()
	UAkAudioEvent* InterruptEvent = nullptr;

	UPROPERTY()
	int32 PlayingID = AK_INVALID_PLAYING_ID;

	UPROPERTY()
	bool bIsPlaying = false;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable, Category = "Commentary SubSystem")
	void PlayCommentaryEvent(class UAkAudioEvent* Event, const int priority);

	void PlayCommentaryEvent(UAkAudioEvent* Event, AActor* TargetActor, int priority);

	void RegisterActor(AActor* TargetActor, UAkAudioEvent* CommentaryInterruptEvent);
	
	DECLARE_DELEGATE(FOnCommentaryEventInterrupt);

	FOnCommentaryEventInterrupt OnCommentaryEventInterruptDelegate;

private:
	UFUNCTION()
	void OnEndEvent(UAkEventCallbackInfo* CallbackInfo, UWwiseHandlerComponent* owner);

	UFUNCTION()
	void OnInterruption(UAkEventCallbackInfo* CallbackInfo, UWwiseHandlerComponent* owner);

};
