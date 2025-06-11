// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EPointType.h"
#include "GameFramework/Actor.h"
#include "Allied.h"
#include "CustomNavigationPoint.generated.h"

class APath;
class UAkAudioEvent;

UCLASS()
class DIAMONDPROJECT_API ACustomNavigationPoint : public AActor
{
	GENERATED_BODY()

	UDELEGATE(BlueprintCallable)
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPointEffectEvent);

	
protected:
	
private:
	UPROPERTY()
	int EffectIndex = 0;
	
public:
	// Sets default values for this actor's properties
	ACustomNavigationPoint();

	UPROPERTY(EditAnywhere, Category="NavPoint")
	EPointType PointType;

	UPROPERTY(EditAnywhere, Category="NavPoint", meta=(EditCondition="PointType == EPointType::Choice"))
	TArray<APath*> PathArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NavPoint", meta=(EditCondition="PointType == EPointType::Choice"))
	int WantedPathIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NavPoint", meta=(EditCondition="PointType == EPointType::Crouch"))
	bool bShouldStayCrouched;

	UPROPERTY(EditAnywhere, Category="NavPoint", meta=(EditCondition="PointType == EPointType::Crouch && !bShouldStayCrouched"))
	float TimeToWaitCrouched;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NavPoint")
	bool bShouldWait;

	UPROPERTY(EditAnywhere, Category="NavPoint", meta=(EditCondition="bShouldWait"))
	float TimeToWait;

	UPROPERTY(EditAnywhere, Category="NavPoint")
	bool bShouldPlayDialogue;

	UPROPERTY(EditAnywhere, Category="NavPoint", BlueprintReadOnly, meta=(EditCondition="bShouldPlayDialogue"))
	UAkAudioEvent* Dialogue;
	
	UPROPERTY(BlueprintReadOnly)
	AAllied* Allied;

	UPROPERTY()
	FTimerHandle TimerHandle;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void PointEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayDialogue();

	UFUNCTION(BlueprintCallable)
	void StopTimer();

	UFUNCTION()
	void StartTimer();

	UFUNCTION(BlueprintImplementableEvent)
	void OnArrivingOnPoint();

	
};
