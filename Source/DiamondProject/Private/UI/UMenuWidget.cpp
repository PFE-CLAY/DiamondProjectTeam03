// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


bool UUMenuWidget::Initialize()
{
	Super::Initialize();
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	return false;
}

void UUMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayerController->SetInputMode(FInputModeUIOnly());
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("UwU"));
}

void UUMenuWidget::OpenNewWidget(TSubclassOf<UUserWidget> Widget)
{
	
	NewWidget = CreateWidget<UUserWidget>(GetWorld(), Widget);
	NewWidget->AddToViewport();
}

void UUMenuWidget::OnQuitApplication()
{
	
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController,EQuitPreference::Quit, true );
}

void UUMenuWidget::OpenLevelByName(FName LevelName)
{
	//UGameplayStatics::OpenLevel(GetWorld(), PlayLevelName,true);
	PlayerController->SetInputMode(FInputModeGameOnly());
	if(GetWorld() == nullptr)
		return;
	FLatentActionInfo LatentInfo;
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UUMenuWidget::OnCloseWindow()
{
	PlayerController->SetInputMode(FInputModeGameOnly());
	this->RemoveFromParent();
	
}








