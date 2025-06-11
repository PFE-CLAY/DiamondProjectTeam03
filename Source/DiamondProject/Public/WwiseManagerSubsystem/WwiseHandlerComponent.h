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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHandleCallbackDelegate, EAkCallbackType, CallbackType, UAkCallbackInfo*, CallbackInfo); 
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEndOfEventCallbackDelegate, UAkEventCallbackInfo*, CallbackInfo, UWwiseHandlerComponent*, owner);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMarkerCallbackDelegate, UAkMarkerCallbackInfo*, CallbackInfo, UWwiseHandlerComponent*, owner);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDurationCallbackDelegate, UAkDurationCallbackInfo*, CallbackInfo, UWwiseHandlerComponent*, owner);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStarvationCallbackDelegate, UAkEventCallbackInfo*, CallbackInfo, UWwiseHandlerComponent*, owner);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMusicPlayStartedCallbackDelegate, UAkEventCallbackInfo*, CallbackInfo, UWwiseHandlerComponent*, owner);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMidiEventCallbackDelegate, UAkMIDIEventCallbackInfo*, CallbackInfo, UWwiseHandlerComponent*, owner);

	UPROPERTY(BlueprintAssignable, Category="Wwise|Callback")
	FHandleCallbackDelegate HandleCallbackDelegate;
	
	UFUNCTION()
	void HandleCallback(EAkCallbackType CallbackType, UAkCallbackInfo* CallbackInfo);

	UPROPERTY(BlueprintAssignable, Category="Wwise|Callback")
	FEndOfEventCallbackDelegate EndOfEventCallbackDelegate;
	
	UFUNCTION()
	void OnEndOfEventCallback(UAkEventCallbackInfo* CallbackInfo, UWwiseHandlerComponent* owner);

	UPROPERTY(BlueprintAssignable, Category="Wwise|Callback")
	FMarkerCallbackDelegate MarkerCallbackDelegate;
	
	UFUNCTION(/*BlueprintImplementableEvent, Category = "Wwise|Event"*/)
	void OnMarkerCallback(UAkMarkerCallbackInfo* CallbackInfo, UWwiseHandlerComponent* owner);

	UPROPERTY(BlueprintAssignable, Category="Wwise|Callback")
	FDurationCallbackDelegate DurationCallbackDelegate;
	
	UFUNCTION()
	void OnDurationCallback(UAkDurationCallbackInfo* CallbackInfo, UWwiseHandlerComponent* owner);

	UPROPERTY(BlueprintAssignable, Category="Wwise|Callback")
	FStarvationCallbackDelegate StarvationCallbackDelegate;
	
	UFUNCTION()
	void OnStarvationCallback(UAkEventCallbackInfo* CallbackInfo, UWwiseHandlerComponent* owner);

	UPROPERTY(BlueprintAssignable, Category="Wwise|Callback")
	FMusicPlayStartedCallbackDelegate MusicPlayStartedCallbackDelegate;
	
	UFUNCTION()
	void OnMusicPlayStartedCallback(UAkEventCallbackInfo* CallbackInfo, UWwiseHandlerComponent* owner);

	UPROPERTY(BlueprintAssignable, Category="Wwise|Callback")
	FMidiEventCallbackDelegate MidiEventCallbackDelegate;
	
	UFUNCTION()
	void OnMidiEventCallback(UAkMIDIEventCallbackInfo* CallbackInfo, UWwiseHandlerComponent* owner);

	UFUNCTION(BlueprintCallable, Category = "Wwise|Mask")
	void AddMaskToCallbackMask(EAkCallbackType CallbackType);

	UFUNCTION(BlueprintCallable, Category="Wwise|Mask")
	void RemoveMaskFromCallbackMask(EAkCallbackType CallbackType);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wwise|Mask", meta=(Bitmask, BitmaskEnum = "/Script/AkAudio.EAkCallbackType"))
	int32 CallbackMask;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wwise|Wwise Event")
	int32 LastPlayedID = 0;
};