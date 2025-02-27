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
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Zone")
	// ATriggerableActor* TriggerableActorClass;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Trigger Zone")
	UShapeComponent *ShapeComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trigger Zone")
	EShapeType ShapeType = EShapeType::Box;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Trigger Zone")
	bool bIsTriggered = false;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Zone")
	ATriggerableActor* TriggerableActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when actor overlap the trigger zone
	UFUNCTION()
	virtual void OnOverlapTriggerZone(UPrimitiveComponent* OverlappedComponent, AActor* AtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom Event")
	void OnTriggerZone();

private:
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:
	// Sets default values for this actor's properties
	ATriggerZone();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

