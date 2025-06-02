// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Commentator.generated.h"

class UAkAudioEvent;

UCLASS()
class DIAMONDPROJECT_API ACommentator : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAkAudioEvent* InterruptEvent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	ACommentator();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};