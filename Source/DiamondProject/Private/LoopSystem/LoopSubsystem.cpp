// Fill out your copyright notice in the Description page of Project Settings.


#include "LoopSystem/LoopSubsystem.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "LoopSystem/LevelSelectionSettings.h"
#include "LoopSystem/PreplanAdvice.h"
#include "LoopSystem/PreplanDataWidget.h"
#include "LoopSystem/PreplanStep.h"
#include "UI/GameSettingsSubsystem.h"

void ULoopSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Display, TEXT("ULoopSubsystem::Initialize"));
}

void ULoopSubsystem::ReloadScene()
{
	for (auto PreplanStep : PreplanSteps){
		PreplanStep.Value->PreplanData = nullptr;
	}

	const ULevelSelectionSettings* LevelSelectionSettings = GetDefault<ULevelSelectionSettings>();
	if (LevelSelectionSettings == nullptr) return;
	
	OnSceneReloadEvent.Broadcast();
	if (PreplanDreamSubtitlesArray.IsEmpty())
	{
		bool mainLevelNull = LevelSelectionSettings->MainLevel.IsNull();
		if (!mainLevelNull)
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this, LevelSelectionSettings->MainLevel, false);
			return;
		}
	} else
	{
		bool dreamLevelNull = LevelSelectionSettings->DreamLevel.IsNull(); 
		if (!dreamLevelNull)
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this, LevelSelectionSettings->DreamLevel, false);
			return;
		}
	}
	
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

bool ULoopSubsystem::IsAnyPreviousStepActive(const UPreplanStep* PreplanStep)
{
	if (PreplanStep == nullptr || PreplanStep->PreplanData == nullptr)
		return false;
	
	bool bIsPreviousStepActive = false;
	
	if (PreplanStep->PreplanData->PreviousDatas.Num() == 0){
		bIsPreviousStepActive = true;
	} else{
		for (TObjectPtr<UPreplanDataWidget> PreviousData : PreplanStep->PreplanData->PreviousDatas) {
			if (PreviousData == nullptr)
				continue;
				
			TObjectPtr<UPreplanStep>* PreviousStep = PreplanSteps.Find(PreviousData->PreplanID);
			if (PreviousStep != nullptr &&
				PreviousStep->Get() != nullptr &&
				PreviousStep->Get()->bIsStepActive){
				bIsPreviousStepActive = true;
				break;
			}
		}
	}
	return bIsPreviousStepActive;
}

void ULoopSubsystem::InitializePreplanAdvices()
{
	TArray<AActor*> FoundPreplanAdviceActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APreplanAdvice::StaticClass(), FoundPreplanAdviceActors);

	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance == nullptr) return;

	UGameSettingsSubsystem* SettingsSubsystem = GameInstance->GetSubsystem<UGameSettingsSubsystem>();
	if (SettingsSubsystem == nullptr) return;

	for (int i = 0; i < FoundPreplanAdviceActors.Num(); i++)
	{
		APreplanAdvice* PreplanDataActor = Cast<APreplanAdvice>(FoundPreplanAdviceActors[i]);
		if (PreplanDataActor == nullptr){
			continue;
		}
			
		TObjectPtr<UPreplanStep>* PreplanStepPtr = PreplanSteps.Find(PreplanDataActor->PreplanID);
		if (PreplanStepPtr == nullptr || PreplanStepPtr->Get() == nullptr){
			return;
		}

		PreplanDataActor->SetActorHiddenInGame(
			!SettingsSubsystem->IsPreplanInSceneVisible() ||
			!PreplanStepPtr->Get()->bIsStepActive);
		PreplanStepPtr->Get()->PreplanAdvices.Add(PreplanDataActor);
	}
}

void ULoopSubsystem::SetPreplanVisibility(UPreplanDataWidget* PreplanData, bool bIsVisible)
{
	if (PreplanData == nullptr) return;
	
	if (bIsVisible)
	{
		PreplanData->SetVisibility(ESlateVisibility::Visible);
	} else
	{
		PreplanData->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ULoopSubsystem::OnAdvicesVisibilityChanged(bool bNewVisibility)
{
	for (auto PreplanStep : PreplanSteps) {
		for (auto PreplanAdvice : PreplanStep.Value->PreplanAdvices) {
			bool value = !bNewVisibility || !PreplanStep.Value->bIsStepVisible;
			PreplanAdvice->SetActorHiddenInGame(value);
		}
	}
}

void ULoopSubsystem::InitializePreplan()
{
	TArray<UUserWidget*> FoundPreplanDataActors;

	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundPreplanDataActors, UPreplanDataWidget::StaticClass(),false);

	if (!bIsInit){
		bIsInit = true;

		for (int i = 0; i < FoundPreplanDataActors.Num(); i++)
		{
			UPreplanDataWidget* PreplanDataActor = Cast<UPreplanDataWidget>(FoundPreplanDataActors[i]);
			if (PreplanDataActor == nullptr){
				continue;
			}
			
			UPreplanStep* PreplanStep = NewObject<UPreplanStep>();
			PreplanStep->PreplanData = PreplanDataActor;
			
			if (PreplanStep->PreplanData->PreplanID.IsEmpty()){
				UE_LOG(LogTemp, Warning, TEXT("Preplan has no ID. Step cannot be created."))				
			} else {
				if (PreplanDataActor->bIsActiveOnStart){
					PreplanStep->bIsStepActive = true;
					PreplanStep->bIsStepVisible = true;
				}
				PreplanStep->PreplanData->SetVisibility(ESlateVisibility::Visible);
				SetPreplanVisibility(PreplanStep->PreplanData,PreplanDataActor->bIsActiveOnStart);
				PreplanSteps.Add(PreplanDataActor->PreplanID,PreplanStep);
			}
		}
		UGameInstance* GameInstance = GetWorld()->GetGameInstance();
		if (GameInstance == nullptr) return;
	
		UGameSettingsSubsystem* SettingsSubsystem = GameInstance->GetSubsystem<UGameSettingsSubsystem>();
		if (SettingsSubsystem == nullptr) return;
		
		SettingsSubsystem->OnAdvicesVisibilityChangedDelegate.AddUniqueDynamic(this, &ULoopSubsystem::OnAdvicesVisibilityChanged);
		
	} else {
		for (int i = 0; i < FoundPreplanDataActors.Num(); i++)
		{
			UPreplanDataWidget* PreplanDataActor = Cast<UPreplanDataWidget>(FoundPreplanDataActors[i]);
			if (PreplanDataActor == nullptr){
				continue;
			}
			
			if (PreplanDataActor->PreplanID.IsEmpty()){
				UE_LOG(LogTemp, Warning, TEXT("Preplan has no ID. Step cannot be found."))				
			} else{
				TObjectPtr<UPreplanStep>* PreplanStepPtr = PreplanSteps.Find(PreplanDataActor->PreplanID);
				if (PreplanStepPtr == nullptr){
					return;
				}
				
				TObjectPtr<UPreplanStep> PreplanStep = PreplanStepPtr->Get();
				if (PreplanStep != nullptr) {
					PreplanStep->NbActivations = 0;
					PreplanStep->PreplanData = PreplanDataActor;
					PreplanStep->PreplanAdvices.Empty();
					PreplanStep->bIsStepVisible = PreplanStep->bIsStepActive;
					SetPreplanVisibility(PreplanDataActor, PreplanStep->bIsStepVisible);
				}
			}
		}
	}
	InitializePreplanAdvices();
}

void ULoopSubsystem::ActivatePreplanStep(FString PreplanID)
{
	const TObjectPtr<UPreplanStep>* PreplanStepPtr = PreplanSteps.Find(PreplanID);
	if (PreplanStepPtr == nullptr){
		return;
	}
	
	TObjectPtr<UPreplanStep> PreplanStep = PreplanStepPtr->Get();

	if (PreplanStep->bIsStepActive)
	{
		return;
	}
	
	bool bIsAnyPreviousStepActive = IsAnyPreviousStepActive(PreplanStep);

	if (bIsAnyPreviousStepActive &&
		PreplanStep->NbActivations < PreplanStep->PreplanData->NbActivationsRequired){
		++PreplanStep->NbActivations;

		if (PreplanStep->NbActivations == PreplanStep->PreplanData->NbActivationsRequired){
			PreplanStep->bIsStepActive = true;
			SetPreplanVisibility(PreplanStep->PreplanData, false);
			//PreplanStep->PreplanData->SetActorHiddenInGame(false);
			
			if (PreplanStep->PreplanData->bShouldActivateDream &&
				PreplanStep->PreplanData->DreamSubtitles != nullptr)
			{
				PreplanDreamSubtitlesArray.Add(PreplanStep->PreplanData->DreamSubtitles);
			}
		}
	}
}
