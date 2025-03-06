// Fill out your copyright notice in the Description page of Project Settings.


#include "LoopSystem/LoopSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "LoopSystem/PreplanData.h"

void ULoopSubsystem::ReloadScene()
{
	for (auto PreplanStep : PreplanSteps){
		PreplanStep.Value->PreplanData = nullptr;
	}
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void ULoopSubsystem::InitializePreplan()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APreplanData::StaticClass(), FoundActors);

	if (!bIsInit){
		bIsInit = true;
		
		for (int i = 0; i < FoundActors.Num(); i++)
		{
			APreplanData* PreplanDataActor = Cast<APreplanData>(FoundActors[i]);
			if (PreplanDataActor == nullptr) continue;
			UPreplanStep* PreplanStep = NewObject<UPreplanStep>();
			PreplanStep->PreplanData = PreplanDataActor;
			if (PreplanStep->PreplanData->PreplanID.IsEmpty()){
				UE_LOG(LogTemp, Warning, TEXT("Preplan has no ID. Step cannot be created."))				
			} else{
				if (PreplanDataActor->bIsActiveOnStart){
					PreplanStep->bIsStepActive = true;
					PreplanStep->PreplanData->SetActorHiddenInGame(false);
				} else{
					PreplanStep->PreplanData->SetActorHiddenInGame(true);
				}
				PreplanSteps.Add(PreplanDataActor->PreplanID,PreplanStep);
			}
		}
	} else {
		for (int i = 0; i < FoundActors.Num(); i++)
		{
			APreplanData* PreplanDataActor = Cast<APreplanData>(FoundActors[i]);
			if (PreplanDataActor == nullptr) continue;
			
			if (PreplanDataActor->PreplanID.IsEmpty()){
				UE_LOG(LogTemp, Warning, TEXT("Preplan has no ID. Step cannot be found."))				
			} else{
				TObjectPtr<UPreplanStep>* PreplanStep = PreplanSteps.Find(PreplanDataActor->PreplanID);
				PreplanStep->Get()->PreplanData = PreplanDataActor;
				bool test = PreplanStep->Get()->bIsStepActive;
				PreplanDataActor->SetActorHiddenInGame(!PreplanStep->Get()->bIsStepActive);
			}
		}
	}
}

void ULoopSubsystem::ActivatePreplanStep(FString PreplanID)
{
	TObjectPtr<UPreplanStep>* PreplanStepPtr = PreplanSteps.Find(PreplanID);
	if (PreplanStepPtr == nullptr)
		return;
	
	TObjectPtr<UPreplanStep> PreplanStep = PreplanStepPtr->Get();
	bool bIsPreviousStepActive = false;
	if (PreplanStep == nullptr || PreplanStep->PreplanData == nullptr)
		return;
	
	if (PreplanStep->PreplanData->PreviousDatas.Num() == 0){
		bIsPreviousStepActive = true;
	} else{
		for (TObjectPtr<APreplanData> PreviousData : PreplanStep->PreplanData->PreviousDatas) {
			TObjectPtr<UPreplanStep>* PreviousStep = PreplanSteps.Find(PreviousData->PreplanID);
			if (PreviousStep->Get()->bIsStepActive){
				bIsPreviousStepActive = true;
				break;
			}
		}
	}
	if (bIsPreviousStepActive &&
		PreplanStep->NbActivations < PreplanStep->PreplanData->NbActivationsRequired){
		++PreplanStep->NbActivations;
		if (PreplanStep->NbActivations == PreplanStep->PreplanData->NbActivationsRequired){
			PreplanStep->bIsStepActive = true;
			PreplanStep->PreplanData->SetActorHiddenInGame(false);
		}
	}
}
