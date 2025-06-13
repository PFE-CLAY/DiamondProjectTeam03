// Fill out your copyright notice in the Description page of Project Settings.


#include "LoopSystem/LoopSubsystem.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "LoopSystem/LevelSelectionSettings.h"
#include "LoopSystem/PreplanAdvice.h"
#include "LoopSystem/PreplanDataWidget.h"
#include "LoopSystem/PreplanLinkWidget.h"
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
	
	if (PreplanStep->InLinks.Num() == 0){
		bIsPreviousStepActive = true;
	} else{
		for (TObjectPtr<UPreplanLinkWidget> PreviousData : PreplanStep->InLinks) {
			if (PreviousData == nullptr || PreviousData->FromPreplanData == nullptr)
				continue;
			
			TObjectPtr<UPreplanStep>* PreviousStep = PreplanSteps.Find(PreviousData->FromPreplanData->PreplanID);
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

void ULoopSubsystem::InitializePreplanSteps()
{
	TArray<UUserWidget*> FoundPreplanDataWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundPreplanDataWidgets, UPreplanDataWidget::StaticClass(),false);
	
	if (!bIsInit){
		bIsInit = true;

		for (int i = 0; i < FoundPreplanDataWidgets.Num(); i++)
		{
			UPreplanDataWidget* PreplanDataWidget = Cast<UPreplanDataWidget>(FoundPreplanDataWidgets[i]);
			CreatePreplanStep(PreplanDataWidget);
		}
		UGameInstance* GameInstance = GetWorld()->GetGameInstance();
		if (GameInstance == nullptr) return;
	
		UGameSettingsSubsystem* SettingsSubsystem = GameInstance->GetSubsystem<UGameSettingsSubsystem>();
		if (SettingsSubsystem == nullptr) return;
		
		SettingsSubsystem->OnAdvicesVisibilityChangedDelegate.AddUniqueDynamic(this, &ULoopSubsystem::OnAdvicesVisibilityChanged);
		
	} else {
		for (int i = 0; i < FoundPreplanDataWidgets.Num(); i++)
		{
			UPreplanDataWidget* PreplanDataActor = Cast<UPreplanDataWidget>(FoundPreplanDataWidgets[i]);
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
					PreplanStep->InLinks.Empty();
					PreplanStep->OutLinks.Empty();
					PreplanStep->bIsStepVisible = PreplanStep->bIsStepActive;
					SetPreplanVisibility(PreplanDataActor, PreplanStep->bIsStepVisible);
				}
			}
		}
	}
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
			PreplanDataActor->SetActorHiddenInGame(
			!SettingsSubsystem->IsPreplanInSceneVisible());
		} else
		{
			PreplanDataActor->SetActorHiddenInGame(
			!SettingsSubsystem->IsPreplanInSceneVisible() ||
			!PreplanStepPtr->Get()->bIsStepActive);
			PreplanStepPtr->Get()->PreplanAdvices.Add(PreplanDataActor);
		}

	}
}

void ULoopSubsystem::InitializePreplanLinks()
{
	TArray<UUserWidget*> FoundPreplanLinkWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundPreplanLinkWidgets, UPreplanLinkWidget::StaticClass(),false);

	for (UUserWidget* Widget : FoundPreplanLinkWidgets)
	{
		if (Widget == nullptr)
		{
			continue;
		}
		if (UPreplanLinkWidget* PreplanLinkWidget = Cast<UPreplanLinkWidget>(Widget))
		{
			if (PreplanLinkWidget->FromPreplanData)
			{
				FString PreplanID = PreplanLinkWidget->FromPreplanData->PreplanID;
				TObjectPtr<UPreplanStep>* PreplanStepPtr = PreplanSteps.Find(PreplanID);
				if (PreplanStepPtr == nullptr || PreplanStepPtr->Get() == nullptr){
					continue;
				}
				
				PreplanStepPtr->Get()->OutLinks.Add(PreplanLinkWidget);
				if (PreplanStepPtr->Get()->bIsStepActive)
				{
					PreplanLinkWidget->ActivateFromData();
				}
			} else
			{
				PreplanLinkWidget->ActivateFromData();
			}

			if (PreplanLinkWidget->ToPreplanData)
			{
				FString PreplanID = PreplanLinkWidget->ToPreplanData->PreplanID;
				TObjectPtr<UPreplanStep>* PreplanStepPtr = PreplanSteps.Find(PreplanID);
				if (PreplanStepPtr == nullptr || PreplanStepPtr->Get() == nullptr){
					continue;
				}
				PreplanStepPtr->Get()->InLinks.Add(PreplanLinkWidget);
				if (PreplanStepPtr->Get()->bIsStepActive)
				{
					PreplanLinkWidget->ActivateToData();
				}
			} else
			{
				PreplanLinkWidget->ActivateToData();
			}

			PreplanLinkWidget->ChangeVisibility(PreplanLinkWidget->IsLinkActive());
		}
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

void ULoopSubsystem::CreatePreplanStep(UPreplanDataWidget* PreplanDataWidget)
{
	if (PreplanDataWidget == nullptr)
	{
		return;
	}
	
	UPreplanStep* PreplanStep = NewObject<UPreplanStep>();
	PreplanStep->PreplanData = PreplanDataWidget;
			
	if (PreplanStep->PreplanData->PreplanID.IsEmpty()){
		UE_LOG(LogTemp, Warning, TEXT("Preplan has no ID. Step cannot be created."))				
	} else {
		if (PreplanDataWidget->bIsActiveOnStart){
			PreplanStep->bIsStepActive = true;
			PreplanStep->bIsStepVisible = true;
		}
		PreplanStep->PreplanData->SetVisibility(ESlateVisibility::Visible);
		SetPreplanVisibility(PreplanStep->PreplanData,PreplanDataWidget->bIsActiveOnStart);
		PreplanSteps.Add(PreplanDataWidget->PreplanID,PreplanStep);
		PreplanDataWidget->SetStep(PreplanStep);
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
	InitializePreplanSteps();
	InitializePreplanAdvices();
	InitializePreplanLinks();
}

void ULoopSubsystem::ActivatePreplanStep(FString PreplanID)
{
	const TObjectPtr<UPreplanStep>* PreplanStepPtr = PreplanSteps.Find(PreplanID);
	if (PreplanStepPtr == nullptr)
	{
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
			
			for (TObjectPtr<UPreplanLinkWidget> InLink : PreplanStep->InLinks)
			{
				InLink->ActivateToData();
			}

			for (TObjectPtr<UPreplanLinkWidget> OutLink : PreplanStep->OutLinks)
			{
				OutLink->ActivateFromData();
			}
			
			if (PreplanStep->PreplanData->bShouldActivateDream &&
				PreplanStep->PreplanData->DreamSubtitles != nullptr)
			{
				PreplanDreamSubtitlesArray.Add(PreplanStep->PreplanData->DreamSubtitles);
			}
		}
	}
}
