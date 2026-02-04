// Fill out your copyright notice in the Description page of Project Settings.


#include "LoopSystem/LoopSubsystem.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "LoopSystem/Collectible.h"
#include "LoopSystem/LevelSelectionSettings.h"
#include "LoopSystem/PreplanAdvice.h"
#include "LoopSystem/PreplanDataWidget.h"
#include "LoopSystem/PreplanLinkWidget.h"
#include "LoopSystem/PreplanStep.h"
#include "UI/GameSettingsSubsystem.h"
#include "SaveSystem/DiamondSaveGame.h"
#include "steam/steam_api.h"

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

	// const ULevelSelectionSettings* LevelSelectionSettings = GetDefault<ULevelSelectionSettings>();
	// if (LevelSelectionSettings == nullptr) return;
	LoopNb++;
	CheckLoopCountAchievement();
	OnSceneReloadEvent.Broadcast();
	// bool mainLevelNull = LevelSelectionSettings->MainLevel.IsNull();
	// if (!mainLevelNull)
	// {
	// 	UGameplayStatics::OpenLevelBySoftObjectPtr(this, LevelSelectionSettings->MainLevel, false);
	// 	return;
	// }
	
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	SaveLoopData();
}

/*bool ULoopSubsystem::IsAnyPreviousStepActive(const UPreplanStep* PreplanStep)
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
}*/
void ULoopSubsystem::InitializeCollectibles()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Acollectible::StaticClass(), FoundActors);
	if (LoopNb==0){
		bIsCollectibleInit=true;
		for (int i = 0; i < FoundActors.Num(); i++){
			Acollectible* Collectible= Cast<Acollectible>(FoundActors[i]);
			CreateCollectible(Collectible);
		}
	}else{
		for (int i = 0; i < FoundActors.Num(); i++){
			Acollectible* Collectible= Cast<Acollectible>(FoundActors[i]);
			if (Collectible == nullptr){
				continue;
			}
			
			if (Collectible->CollectibleID.IsEmpty()){
				UE_LOG(LogTemp, Warning, TEXT("Collectible has no ID. Collectible cannot be found."))				
			} else{
				if (Collectibles.Contains(Collectible->CollectibleID))
				{
					Collectible->bHasBeenCollected=Collectibles[Collectible->CollectibleID];
				}
			}
		}
	}
}

void ULoopSubsystem::InitializePreplanSteps()
{
	TArray<UUserWidget*> FoundPreplanDataWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundPreplanDataWidgets, UPreplanDataWidget::StaticClass(),false);
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::FromInt(FoundPreplanDataWidgets.Num()));
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
			UPreplanDataWidget* PreplanDataWidget = Cast<UPreplanDataWidget>(FoundPreplanDataWidgets[i]);
			if (PreplanDataWidget == nullptr){
				continue;
			}
			
			if (PreplanDataWidget->PreplanID.IsEmpty()){
				UE_LOG(LogTemp, Warning, TEXT("Preplan has no ID. Step cannot be found."))				
			} else{
				TObjectPtr<UPreplanStep>* PreplanStepPtr = PreplanSteps.Find(PreplanDataWidget->PreplanID);
				if (PreplanStepPtr == nullptr){
					return;
				}
				
				TObjectPtr<UPreplanStep> PreplanStep = PreplanStepPtr->Get();
				if (PreplanStep != nullptr) {
					PreplanStep->FirstNbActivations=0;
					PreplanStep->SndNbActivations=0;
					PreplanStep->PreplanData = PreplanDataWidget;
					PreplanStep->PreplanAdvices.Empty();
					//PreplanStep->InLinks.Empty();
					//PreplanStep->OutLinks.Empty();
					//SetPreplanVisibility(PreplanDataWidget, PreplanStep->bIsStepVisible);
					PreplanDataWidget->SetStep(PreplanStep);
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

/*void ULoopSubsystem::InitializePreplanLinks()
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
				
				//PreplanStepPtr->Get()->OutLinks.Add(PreplanLinkWidget);
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
				//PreplanStepPtr->Get()->InLinks.Add(PreplanLinkWidget);
				if (PreplanStepPtr->Get()->bIsStepActive)
				{
					PreplanLinkWidget->ActivateToData();
				}
			} else
			{
				PreplanLinkWidget->ActivateToData();
			}

			//PreplanLinkWidget->ChangeVisibility(PreplanLinkWidget->IsLinkActive());
		}
	}
}*/

/*void ULoopSubsystem::SetPreplanVisibility(UPreplanDataWidget* PreplanData, bool bIsVisible)
{
	if (PreplanData == nullptr) return;
	
	if (bIsVisible)
	{
		PreplanData->SetVisibility(ESlateVisibility::Visible);
	} else
	{
		PreplanData->SetVisibility(ESlateVisibility::Hidden);
	}
}*/

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
			PreplanStep->bIsFirstStepVisible = true;
		}
		//PreplanStep->PreplanData->SetVisibility(ESlateVisibility::Visible);
		//SetPreplanVisibility(PreplanStep->PreplanData,PreplanDataWidget->bIsActiveOnStart);
		PreplanSteps.Add(PreplanDataWidget->PreplanID,PreplanStep);
		PreplanDataWidget->SetStep(PreplanStep);
	}
}

void ULoopSubsystem::CreateCollectible(const Acollectible* Collectible)
{
	if (Collectible == nullptr)
	{
		return;
	}
	Collectibles.Add(Collectible->CollectibleID,Collectible->bHasBeenCollected);
}

void ULoopSubsystem::OnAdvicesVisibilityChanged(bool bNewVisibility)
{
	for (auto PreplanStep : PreplanSteps) {
		for (auto PreplanAdvice : PreplanStep.Value->PreplanAdvices) {
			//bool value = !bNewVisibility || !PreplanStep.Value->bIsStepVisible;
			//PreplanAdvice->SetActorHiddenInGame(value);
		}
	}
}

void ULoopSubsystem::InitializePreplan()
{
	InitializePreplanSteps();
	InitializeCollectibles();
	InitializePreplanAdvices();
	LoadLoopData();
	//InitializePreplanLinks();

}

void ULoopSubsystem::ActivatePreplanStep(FString PreplanID,int StepPart)
{
	const TObjectPtr<UPreplanStep>* PreplanStepPtr = PreplanSteps.Find(PreplanID);
	if (PreplanStepPtr == nullptr)
	{
		return;
	}
	
	TObjectPtr<UPreplanStep> PreplanStep = PreplanStepPtr->Get();

	if (PreplanStep->bIsSndStepVisible||(PreplanStep->bIsStepActive&&StepPart==1))
	{
		return;
	}
	
	//bool bIsAnyPreviousStepActive = IsAnyPreviousStepActive(PreplanStep);
	if (StepPart == 1){
	
		if (PreplanStep->FirstNbActivations < PreplanStep->PreplanData->NbActivationsRequired){
			++PreplanStep->FirstNbActivations;

			if (PreplanStep->FirstNbActivations == PreplanStep->PreplanData->NbActivationsRequired)
			{
				PreplanStep->bIsStepActive = true;
				PreplanStep->bIsFirstStepVisible = true;
			}
			/*for (TObjectPtr<UPreplanLinkWidget> InLink : PreplanStep->InLinks)
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
			}*/
	
		}
	}
	else if (StepPart == 2)
	{
		if (PreplanStep->SndNbActivations < PreplanStep->PreplanData->SndNbActivationsRequired)
		{
			++PreplanStep->SndNbActivations;

			if (PreplanStep->SndNbActivations == PreplanStep->PreplanData->SndNbActivationsRequired)
			{
				PreplanStep->bIsStepActive = true;
				PreplanStep->bIsSndStepVisible = true;
			}
		}
	}
	SaveLoopData();
	CheckAllPreplanStepsUnlocked();
}

void ULoopSubsystem::ActivateCollectible(FString CollectibleID)
{
	if (CollectibleID.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ActivateCollectible called with empty CollectibleID."));
		return;
	}

	if (Collectibles.Contains(CollectibleID)) // Key exists
	{
		Collectibles[CollectibleID] = true;
	}
	else // Key doesnt exist
	{
		Collectibles.Add(CollectibleID, true);
	}

	SaveLoopData();
}

void ULoopSubsystem::SaveLoopData()
{
	UDiamondSaveGame* SaveGameInstance = Cast<UDiamondSaveGame>(UGameplayStatics::LoadGameFromSlot("DiamondSaveSlot", 0));
	if (!SaveGameInstance)
	{
		SaveGameInstance = Cast<UDiamondSaveGame>(UGameplayStatics::CreateSaveGameObject(UDiamondSaveGame::StaticClass()));
	}

	if (SaveGameInstance)
	{
		SaveGameInstance->LoopData.LoopNb = LoopNb;
		SaveGameInstance->LoopData.IsDeadByTimer = bIsDeadByTimer;
		SaveGameInstance->LoopData.Collectibles = Collectibles;
		SaveGameInstance->LoopData.bIsDreamLevel = bIsDreamLevel;
		SaveGameInstance->LoopData.TotalEnemyKillCount = TotalEnemyKillCount;

		SaveGameInstance->LoopData.PreplanSteps.Empty();
		for (const TPair<FString, TObjectPtr<UPreplanStep>>& Pair : PreplanSteps)
		{
			if (Pair.Value)
			{
				FPreplanStepSaveData StepData;
				StepData.FirstNbActivations = Pair.Value->FirstNbActivations;
				StepData.SndNbActivations = Pair.Value->SndNbActivations;
				StepData.bIsStepActive = Pair.Value->bIsStepActive;
				StepData.bIsFirstStepVisible = Pair.Value->bIsFirstStepVisible;
				StepData.bIsFirstAlreadySeen = Pair.Value->bIsFirstAlreadySeen;
				StepData.bIsSndStepVisible = Pair.Value->bIsSndStepVisible;
				StepData.bIsSndAlreadySeen = Pair.Value->bIsSndAlreadySeen;

				SaveGameInstance->LoopData.PreplanSteps.Add(Pair.Key, StepData);
			}
		}

		UGameplayStatics::SaveGameToSlot(SaveGameInstance, "DiamondSaveSlot", 0);
	}
}

void ULoopSubsystem::LoadLoopData()
{
	UDiamondSaveGame* SaveGameInstance = Cast<UDiamondSaveGame>(UGameplayStatics::LoadGameFromSlot("DiamondSaveSlot", 0));
	if (SaveGameInstance)
	{
		LoopNb = SaveGameInstance->LoopData.LoopNb;
		bIsDeadByTimer = SaveGameInstance->LoopData.IsDeadByTimer;
		//Collectibles = SaveGameInstance->LoopData.Collectibles;
		bIsDreamLevel = SaveGameInstance->LoopData.bIsDreamLevel;
		TotalEnemyKillCount = SaveGameInstance->LoopData.TotalEnemyKillCount;
		
		for (const TPair<FString, FPreplanStepSaveData>& Pair : SaveGameInstance->LoopData.PreplanSteps)
		{
			if (PreplanSteps.Contains(Pair.Key))
			{
				UPreplanStep* Step = PreplanSteps[Pair.Key];
				if (Step)
				{
					Step->FirstNbActivations = Pair.Value.FirstNbActivations;
					Step->SndNbActivations = Pair.Value.SndNbActivations;
					Step->bIsStepActive = Pair.Value.bIsStepActive;
					Step->bIsFirstStepVisible = Pair.Value.bIsFirstStepVisible;
					Step->bIsFirstAlreadySeen = Pair.Value.bIsFirstAlreadySeen;
					Step->bIsSndStepVisible = Pair.Value.bIsSndStepVisible;
					Step->bIsSndAlreadySeen = Pair.Value.bIsSndAlreadySeen;
				}
			}
		}
		for (const TPair<FString, bool>& Pair : SaveGameInstance->LoopData.Collectibles)
		{
			Collectibles.Add(Pair.Key, Pair.Value);;
		}
	}
}

void ULoopSubsystem::ResetAllProgress()
{
	LoopNb = 0;
	bIsDeadByTimer = false;
	Collectibles.Empty();
	bIsDreamLevel = false;
	TotalEnemyKillCount = 0;

	for (auto& Pair : PreplanSteps)
	{
		UPreplanStep* Step = Pair.Value;
		if (Step && Step->PreplanData)
		{
			Step->FirstNbActivations = 0;
			Step->SndNbActivations = 0;
			Step->bIsStepActive = Step->PreplanData->bIsActiveOnStart;
			Step->bIsFirstStepVisible = Step->PreplanData->bIsActiveOnStart;
			Step->bIsFirstAlreadySeen = false;
			Step->bIsSndStepVisible = false;
			Step->bIsSndAlreadySeen = false;
		}
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Acollectible::StaticClass(), FoundActors);
	for (int i = 0; i < FoundActors.Num(); i++){
		Acollectible* Collectible= Cast<Acollectible>(FoundActors[i]);
		if (Collectible)
		{
			Collectible->bHasBeenCollected = false;
		}
	}

	SaveLoopData();
}

bool ULoopSubsystem::IsLoopNbInSaveZero()
{
	UDiamondSaveGame* SaveGameInstance = Cast<UDiamondSaveGame>(UGameplayStatics::LoadGameFromSlot("DiamondSaveSlot", 0));
	if (SaveGameInstance)
	{
		return SaveGameInstance->LoopData.LoopNb == 0;
	}
	return true;
}

void ULoopSubsystem::CheckAllPreplanStepsUnlocked()
{
	if (bHasUnlockedAllPreplanStepsAchievement)
	{
		return;
	}

	if (PreplanSteps.Num() == 0)
	{
		return;
	}

	bool bAllUnlocked = true;
	for (const TPair<FString, TObjectPtr<UPreplanStep>>& Pair : PreplanSteps)
	{
		if (Pair.Value && !Pair.Value->bIsStepActive)
		{
			bAllUnlocked = false;
			break;
		}
	}

	if (bAllUnlocked)
	{
		bHasUnlockedAllPreplanStepsAchievement = true;

#if !UE_SERVER
		if (SteamAPI_Init() && SteamUserStats())
		{
			SteamUserStats()->SetAchievement("ACH_COMPLETIONIST");
			SteamUserStats()->StoreStats();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to unlock achievement ACH_COMPLETIONIST - Steam not initialized"));
		}
#endif
	}
}

void ULoopSubsystem::CheckLoopCountAchievement()
{
	if (bHasUnlockedWildsAchievement)
	{
		return;
	}

	if (LoopNb >= 22)
	{
		bHasUnlockedWildsAchievement = true;

#if !UE_SERVER
		if (SteamAPI_Init() && SteamUserStats())
		{
			SteamUserStats()->SetAchievement("ACH_WILDS");
			SteamUserStats()->StoreStats();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to unlock achievement ACH_WILDS - Steam not initialized"));
		}
#endif
	}
}

void ULoopSubsystem::OnEnemyKilled()
{
	TotalEnemyKillCount++;
	CheckEnemyKillCountAchievement();
	SaveLoopData();
}

void ULoopSubsystem::CheckEnemyKillCountAchievement()
{
	if (bHasUnlockedGetOffMeAchievement)
	{
		return;
	}

	if (TotalEnemyKillCount >= 500)
	{
		bHasUnlockedGetOffMeAchievement = true;

#if !UE_SERVER
		if (SteamAPI_Init() && SteamUserStats())
		{
			SteamUserStats()->SetAchievement("ACH_GETOFFME");
			SteamUserStats()->StoreStats();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to unlock achievement ACH_GETOFFME - Steam not initialized"));
		}
#endif
	}
}

