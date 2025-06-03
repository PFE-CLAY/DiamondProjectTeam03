// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PreplanLinkWidget.generated.h"

class UPreplanDataWidget;
/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API UPreplanLinkWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UPreplanDataWidget> FromPreplanData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UPreplanDataWidget> ToPreplanData;
	
private:
	UPROPERTY()
	bool bInDataActive = false;
	
	UPROPERTY()
	bool bOutDataActive = false;

public:
	UFUNCTION()
	bool IsLinkActive() const;

	UFUNCTION()
	void ActivateFromData();

	UFUNCTION()
	void ActivateToData();

	UFUNCTION()
	void ChangeVisibility(bool bIsVisible);
};
