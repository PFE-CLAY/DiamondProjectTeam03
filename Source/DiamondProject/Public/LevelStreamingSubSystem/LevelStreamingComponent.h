// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelStreamingComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIAMONDPROJECT_API ULevelStreamingComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category="Level Streaming")
	TArray<UWorld*> LevelToLoad;

	UPROPERTY()
	TArray<ALevelStreamingVolume*> FoundStreamingVolumes;

	UPROPERTY()
	bool bVolumesEnabled = true;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	// Sets default values for this component's properties
	ULevelStreamingComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Level Streaming Component")
	void ForceLoadLevel() const;

	UFUNCTION(BlueprintCallable, Category="Level Streaming Component")
	void ForceLoadLevels(TArray<UWorld*> Levels) const;
	
	UFUNCTION(BlueprintCallable, Category="Level Streaming Component")
	void ForceUnloadLevel() const;

	UFUNCTION(BlueprintCallable, Category="Level Streaming Component")
	void ForceUnloadLevels(TArray<UWorld*> Levels) const;

	UFUNCTION(BlueprintCallable, Category="Level Streaming Component")
	void EnableStreamingVolumes(bool Enable) const;
};