// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriggerableActor.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "DiamondProject/ShapeType.h"
#include "TriggerZone.generated.h"

UCLASS()
class DIAMONDPROJECT_API ATriggerZone : public AActor
{
	GENERATED_BODY()
	
protected:
	// Used to determine if the trigger zone can be triggered multiple times
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trigger Zone")
	bool bIsOnce = false;

	// Determine the time between each trigger
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trigger Zone", meta=(EditCondition="ShowTimeBetweenTrigger", EditConditionHides))
	float TimeBetweenTrigger = 0.0f;	

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Trigger Zone")
	float TimeElapseToBeTriggered = 0.0; 
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Trigger Zone")
	UShapeComponent *ShapeComponent = nullptr;
	
	// Used to tell if the trigger zone is triggered
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Trigger Zone")
	bool bIsTriggered = false;

public:
	// The actor that will be triggered
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Zone")
	ATriggerableActor* TriggerableActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when actor overlap the trigger zone
	UFUNCTION()
	virtual void OnOverlapTriggerZone(UPrimitiveComponent* OverlappedComponent, AActor* AtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Used to show the TimeBetweenTrigger property in details of the Blueprint
	UFUNCTION()
	bool ShowTimeBetweenTrigger() const { return !bIsOnce;}

	UFUNCTION()
	virtual void OnUpdate(float DeltaTime);

	UFUNCTION()
	virtual void OnResetUpdate(float DeltaTime);
	
	// Event called when the trigger zone is triggered
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Custom Event")
	void OnTriggerZoneEvent();
	
	virtual void OnTriggerZoneEvent_Implementation();

private:
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:
	// Sets default values for this actor's properties
	ATriggerZone();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

