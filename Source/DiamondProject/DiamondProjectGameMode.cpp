// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiamondProjectGameMode.h"
#include "DiamondProjectCharacter.h"
#include "LoopSystem/LoopSubsystem.h"
#include "UObject/ConstructorHelpers.h"

ADiamondProjectGameMode::ADiamondProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void ADiamondProjectGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitPreplan();
}

void ADiamondProjectGameMode::InitPreplan() const
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance == nullptr) return;

	//ULoopSubsystem* LoopSubsystem = GameInstance->GetSubsystem<ULoopSubsystem>();
	//if (LoopSubsystem == nullptr) return;

	//LoopSubsystem->InitializePreplan();
}
