// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AkAudioEvent.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommentarySubSystem.generated.h"


/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API UCommentarySubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	TPair<UAkAudioEvent*, int> CurrentEvenPlayedWithPriority = {nullptr, 0};
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable, Category = "Commentary SubSystem", meta = (AdvancedDisplay = "3"))
	void PlayCommentaryEvent(UAkAudioEvent* Event, AActor* TargetActor, int priority, const FOnAkPostEventCallback& PostEventCallback);

private:

};
