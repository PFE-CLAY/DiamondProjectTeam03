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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHandleCallbackDelegate, EAkCallbackType, CallbackType, UAkCallbackInfo*, CallbackInfo); 
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEndOfEventCallbackDelegate, UAkEventCallbackInfo*, CallbackInfo);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMarkerCallbackDelegate, UAkMarkerCallbackInfo*, CallbackInfo);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDurationCallbackDelegate, UAkDurationCallbackInfo*, CallbackInfo);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStarvationCallbackDelegate, UAkEventCallbackInfo*, CallbackInfo);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMusicPlayStartedCallbackDelegate, UAkEventCallbackInfo*, CallbackInfo);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMidiEventCallbackDelegate, UAkMIDIEventCallbackInfo*, CallbackInfo);

	UPROPERTY(BlueprintAssignable, Category="Wwise|Callback")
	FHandleCallbackDelegate HandleCallbackDelegate;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Wwise|Event")
	void HandleCallback(EAkCallbackType CallbackType, UAkCallbackInfo* CallbackInfo);

	UPROPERTY(BlueprintAssignable, Category="Wwise|Callback")
	FEndOfEventCallbackDelegate EndOfEventCallbackDelegate;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Wwise|Event")
	void OnEndOfEventCallback(UAkEventCallbackInfo* CallbackInfo);

	UPROPERTY(BlueprintAssignable, Category="Wwise|Callback")
	FMarkerCallbackDelegate MarkerCallbackDelegate;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Wwise|Event")
	void OnMarkerCallback(UAkMarkerCallbackInfo* CallbackInfo);

	UPROPERTY(BlueprintAssignable, Category="Wwise|Callback")
	FDurationCallbackDelegate DurationCallbackDelegate;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Wwise|Event")
	void OnDurationCallback(UAkDurationCallbackInfo* CallbackInfo);

	UPROPERTY(BlueprintAssignable, Category="Wwise|Callback")
	FStarvationCallbackDelegate StarvationCallbackDelegate;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Wwise|Event")
	void OnStarvationCallback(UAkEventCallbackInfo* CallbackInfo);

	UPROPERTY(BlueprintAssignable, Category="Wwise|Callback")
	FMusicPlayStartedCallbackDelegate MusicPlayStartedCallbackDelegate;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Wwise|Event")
	void OnMusicPlayStartedCallback(UAkEventCallbackInfo* CallbackInfo);

	UPROPERTY(BlueprintAssignable, Category="Wwise|Callback")
	FMidiEventCallbackDelegate MidiEventCallbackDelegate;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Wwise|Event")
	void OnMidiEventCallback(UAkMIDIEventCallbackInfo* CallbackInfo);

	void HandleCallback_Implementation(EAkCallbackType CallbackType, UAkCallbackInfo* CallbackInfo);

	void OnEndOfEventCallback_Implementation(UAkEventCallbackInfo* CallbackInfo);

	void OnMarkerCallback_Implementation(UAkMarkerCallbackInfo* CallbackInfo);

	void OnDurationCallback_Implementation(UAkDurationCallbackInfo* CallbackInfo);

	void OnStarvationCallback_Implementation(UAkEventCallbackInfo* CallbackInfo);

	void OnMusicPlayStartedCallback_Implementation(UAkEventCallbackInfo* CallbackInfo);

	void OnMidiEventCallback_Implementation(UAkMIDIEventCallbackInfo* CallbackInfo);

	UFUNCTION(BlueprintCallable, Category = "Wwise|Mask")
	void AddMaskToCallbackMask(EAkCallbackType CallbackType);

	UFUNCTION(BlueprintCallable, Category="Wwise|Mask")
	void RemoveMaskFromCallbackMask(EAkCallbackType CallbackType);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wwise|Mask", meta=(Bitmask, BitmaskEnum = "/Script/AkAudio.EAkCallbackType"))
	int32 CallbackMask;
};

// 5
// 13
// 45
// 173
// 429
// 941
// 1965
// 4013
// 8109
// 16301
// 32685
// 98221