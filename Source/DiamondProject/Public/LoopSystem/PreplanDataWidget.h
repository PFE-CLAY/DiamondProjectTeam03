// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PreplanStep.h"
#include "Blueprint/UserWidget.h"
#include "PreplanDataWidget.generated.h"

/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API UPreplanDataWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PreplanData")
	FString PreplanID;

	UPROPERTY(BlueprintReadOnly,Category="PreplanData")
	TObjectPtr<UPreplanStep> PreplanStep;
	
	UFUNCTION()
	void SetStep(UPreplanStep* Step);

	UPROPERTY(EditAnywhere, Category="PreplanData")
	int NbActivationsRequired = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PreplanData")
	bool bIsActiveOnStart = false;

	UPROPERTY(EditAnywhere, Category="PreplanData")
	bool bShouldActivateDream = false;
	
	UPROPERTY(EditAnywhere, Category="PreplanData", meta = (EditCondition="bShouldActivateDream"))
	TObjectPtr<UDataTable> DreamSubtitles;
};
