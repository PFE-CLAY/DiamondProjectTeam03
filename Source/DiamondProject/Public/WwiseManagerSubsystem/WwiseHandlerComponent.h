// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WwiseHandlerComponent.generated.h"

enum class EAkCallbackType : uint8;
class UAkCallbackInfo;
class UAkMIDIEventCallbackInfo;
class UAkDurationCallbackInfo;
class UAkMarkerCallbackInfo;
class UAkEventCallbackInfo;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIAMONDPROJECT_API UWwiseHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWwiseHandlerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Wwise|Callback")
	void HandleCallback(EAkCallbackType CallbackType, UAkCallbackInfo* CallbackInfo);

	UFUNCTION(BlueprintImplementableEvent, Category = "Wwise|Callback")
	void OnEndOfEventCallback(UAkEventCallbackInfo* CallbackInfo);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Wwise|Callback")
	void OnMarkerCallback(UAkMarkerCallbackInfo* CallbackInfo);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Wwise|Callback")
	void OnDurationCallback(UAkDurationCallbackInfo* CallbackInfo);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Wwise|Callback")
	void OnStarvationCallback(UAkEventCallbackInfo* CallbackInfo);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Wwise|Callback")
	void OnMusicPlayStartedCallback(UAkEventCallbackInfo* CallbackInfo);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Wwise|Callback")
	void OnMidiEventCallback(UAkMIDIEventCallbackInfo* CallbackInfo);

	void HandleCallback_Implementation(EAkCallbackType CallbackType, UAkCallbackInfo* CallbackInfo);

	void OnEndOfEventCallback_Implementation(UAkEventCallbackInfo* CallbackInfo);

	void OnMarkerCallback_Implementation(UAkMarkerCallbackInfo* CallbackInfo);

	void OnDurationCallback_Implementation(UAkDurationCallbackInfo* CallbackInfo);

	void OnStarvationCallback_Implementation(UAkEventCallbackInfo* CallbackInfo);

	void OnMusicPlayStartedCallback_Implementation(UAkEventCallbackInfo* CallbackInfo);

	void OnMidiEventCallback_Implementation(UAkMIDIEventCallbackInfo* CallbackInfo);
};
