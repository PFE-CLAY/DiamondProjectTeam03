// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamingSubSystem/LevelStreamingComponent.h"

#include "Engine/LevelStreamingVolume.h"
#include "Kismet/GameplayStatics.h"
#include "LevelStreamingSubSystem/LevelStreamingSubSystem.h"


// Sets default values for this component's properties
ULevelStreamingComponent::ULevelStreamingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void ULevelStreamingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UWorld* World = GetWorld();
	TArray<AActor *> TempFoundActors;

	UGameplayStatics::GetAllActorsOfClass(World, ALevelStreamingVolume::StaticClass(), TempFoundActors);
	for (AActor* Actor : TempFoundActors) {
		ALevelStreamingVolume* StreamingVolume = Cast<ALevelStreamingVolume>(Actor);
		if (StreamingVolume) {
			FoundStreamingVolumes.Add(StreamingVolume);
		}
	}
	
}


// Called every frame
void ULevelStreamingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULevelStreamingComponent::ForceLoadLevel() const
{
	TObjectPtr<ULevelStreamingSubSystem> LevelStreamingSubSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<ULevelStreamingSubSystem>();
	
	for (TObjectPtr<UWorld> World : LevelToLoad) {
		if (World && !LevelStreamingSubSystem->GetLoadedLevel(World.GetFName())) {
			ULevelStreaming* LevelStreaming = LevelStreamingSubSystem->LoadLevelByName(World->GetLevel(0), World->GetFName(), true, true);
			UE_LOG(LogTemp, Log, TEXT("[ULevelStreamingComponent::ForceLoadLevel] Level %s loaded."), *World->GetName());
		}
	}
}

void ULevelStreamingComponent::ForceLoadLevels(TArray<UWorld*> Levels) const
{
	TObjectPtr<ULevelStreamingSubSystem> LevelStreamingSubSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<ULevelStreamingSubSystem>();
	
	for (TObjectPtr<UWorld> World : Levels) {
		if (World && !LevelStreamingSubSystem->GetLoadedLevel(World.GetFName())) {
			ULevelStreaming* LevelStreaming = LevelStreamingSubSystem->LoadLevelByName(World->GetLevel(0), World->GetFName(), true, true);
			UE_LOG(LogTemp, Log, TEXT("[ULevelStreamingComponent::ForceLoadLevel] Level %s loaded."), *World->GetName());
		}
	}
}

void ULevelStreamingComponent::ForceUnloadLevel() const
{
	TObjectPtr<ULevelStreamingSubSystem> LevelStreamingSubSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<ULevelStreamingSubSystem>();
	
	for (TObjectPtr<UWorld> World : LevelToLoad) {
		if (World && LevelStreamingSubSystem->GetLoadedLevel(World.GetFName())) {
			UGameplayStatics::GetGameInstance(this)->GetSubsystem<ULevelStreamingSubSystem>()->UnloadLevelByName(World->GetFName());
		}
	}
}

void ULevelStreamingComponent::ForceUnloadLevels(TArray<UWorld*> Levels) const
{
	TObjectPtr<ULevelStreamingSubSystem> LevelStreamingSubSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<ULevelStreamingSubSystem>();
	
	for (TObjectPtr<UWorld> World : Levels) {
		if (World && LevelStreamingSubSystem->GetLoadedLevel(World.GetFName())) {
			UGameplayStatics::GetGameInstance(this)->GetSubsystem<ULevelStreamingSubSystem>()->UnloadLevelByName(World->GetFName());
		}
	}
}

void ULevelStreamingComponent::EnableStreamingVolumes(bool Enable) const
{
	for (ALevelStreamingVolume* StreamingVolume : FoundStreamingVolumes) {
		if (StreamingVolume) {
			StreamingVolume->bDisabled = !Enable;
			UE_LOG(LogTemp, Log, TEXT("[ULevelStreamingComponent::EnableStreamingVolumes] Streaming Volume %s collision set to %s."), *StreamingVolume->GetName(), Enable ? TEXT("Enabled") : TEXT("Disabled"));
		}
	}
}

