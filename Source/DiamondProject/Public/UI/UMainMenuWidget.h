// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API UUMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LevelChoiceWidgetClass;

	UPROPERTY()
	APlayerController* PlayerController;
	
private:

	UPROPERTY()
	UUserWidget* LevelChoiceWidget;

	
protected:
	UFUNCTION()
	virtual bool Initialize() override;
	
	UFUNCTION(BlueprintCallable)
	void OnPlayButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnQuitApplication();

	UFUNCTION(BlueprintCallable)
    void OpenLevelByName(FName LevelName);
	
	UFUNCTION(BlueprintCallable)
	void OnCloseWindow();
	
};
