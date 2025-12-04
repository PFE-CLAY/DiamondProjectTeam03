// Copyright Epic Games, Inc. All Rights Reserved.


#include "DiamondProjectPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "UI/GameSettingsSubsystem.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>

class UGameSettingsSubsystem;



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
	case 0x040C: // French (France)
	case 0x080C: // French (Belgium)
		return (uint8)EDetectedLayout::AZERTY; // French-family layouts -> AZERTY

	case 0x0409: // English (United States)
	case 0x0809: // English (United Kingdom)
	case 0x0C09: // English (Australia)
	case 0x1009: // English (Canada)
	case 0x1409: // English (New Zealand)
	case 0x1C09: // English (Ireland)
	case 0x1809: // English (Hong Kong)
		return (uint8)EDetectedLayout::QWERTY; // Common English layouts -> QWERTY

	default:
		return (uint8)EDetectedLayout::AZERTY; // Default to AZERTY
	}
#else
	// On non-Windows platforms, default to AZERTY
	return (uint8)EDetectedLayout::AZERTY;
#endif
}
