// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriggerZone.h"
#include "TimedTriggerZone.generated.h"

UCLASS()
class DIAMONDPROJECT_API ATimedTriggerZone : public ATriggerZone
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Timed Trigger Zone")
	float TimeToTrigger = 5.0f;

private:
	UPROPERTY()
	float TimeElapsed = 0.0f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when actor overlap the trigger zone
	virtual void OnOverlapTriggerZone(UPrimitiveComponent* OverlappedComponent, AActor* AtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// virtual void OnTriggerZoneEvent_Implementation() override;

	virtual void OnUpdate(float DeltaTime) override;

	virtual void OnResetUpdate(float DeltaTime) override;
	
public:
	// Sets default values for this actor's properties
	ATimedTriggerZone();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
