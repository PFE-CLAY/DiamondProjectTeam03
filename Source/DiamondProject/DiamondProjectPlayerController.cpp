// Copyright Epic Games, Inc. All Rights Reserved.


#include "DiamondProjectPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "UI/GameSettingsSubsystem.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>

class UGameSettingsSubsystem;

enum EDetectedLayout : uint8
{
	QWERTY,
	AZERTY,
};

void ADiamondProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		switch (DetectKeyboardLayout()) {
			case EDetectedLayout::QWERTY:
				if (QWERTYInputMappingContext)
				{
					Subsystem->AddMappingContext(QWERTYInputMappingContext, 0);
				}
				break;
			case EDetectedLayout::AZERTY:
				if (AZERTYInputMappingContext)
				{
					Subsystem->AddMappingContext(AZERTYInputMappingContext, 0);
				}
				break;
		}
	}
}

uint8 ADiamondProjectPlayerController::DetectKeyboardLayout()
{
#if PLATFORM_WINDOWS
	HKL KeyboardLayout = GetKeyboardLayout(0);
	WORD LangID = LOWORD(KeyboardLayout);

	switch (LangID)
	{
	case 0x040C: // French
	case 0x080C: // Belgian
		return EDetectedLayout::AZERTY ;// Returns 1

	default:
		return (uint8)EDetectedLayout::QWERTY; // Returns 0
	}
#else
	return (uint8)EDetectedLayout::QWERTY;
#endif
}
