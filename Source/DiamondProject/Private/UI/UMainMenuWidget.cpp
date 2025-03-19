// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UMainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


bool UUMainMenuWidget::Initialize()
{
	Super::Initialize();
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	return false;
}

void UUMainMenuWidget::OnPlayButtonClicked()
{
	
	LevelChoiceWidget = CreateWidget<UUserWidget>(GetWorld(), LevelChoiceWidgetClass);
	LevelChoiceWidget->AddToViewport();
}

void UUMainMenuWidget::OnQuitApplication()
{
	
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController,EQuitPreference::Quit, true );
}

void UUMainMenuWidget::OpenLevelByName(FName LevelName)
{
	//UGameplayStatics::OpenLevel(GetWorld(), PlayLevelName,true);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Clicked"));
	if(GetWorld() == nullptr)
		return;
	FLatentActionInfo LatentInfo;
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UUMainMenuWidget::OnCloseWindow()
{
	this->RemoveFromParent();
}






