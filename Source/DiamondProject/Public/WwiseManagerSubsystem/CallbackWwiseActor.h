// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CallbackWwiseActor.generated.h"

class UAkMIDIEventCallbackInfo;
class UAkDurationCallbackInfo;
class UAkMarkerCallbackInfo;
class UAkEventCallbackInfo;
// This class does not need to be modified.
UINTERFACE()
class UCallbackWwiseActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIAMONDPROJECT_API ICallbackWwiseActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Wwise|Callback")
	void HandleCallback(EAkCallbackType CallbackType, UAkCallbackInfo* CallbackInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Wwise|Callback")
	void OnEndOfEventCallback(UAkEventCallbackInfo* CallbackInfo);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Wwise|Callback")
	void OnMarkerCallback(UAkMarkerCallbackInfo* CallbackInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Wwise|Callback")
	void OnDurationCallback(UAkEventCallbackInfo* CallbackInfo);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Wwise|Callback")
	void OnMusicPlayStartedCallback(UAkCallbackInfo* CallbackInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Wwise|Callback")
	void OnMidiEventCallback(UAkMIDIEventCallbackInfo* CallbackInfo);
	
};
