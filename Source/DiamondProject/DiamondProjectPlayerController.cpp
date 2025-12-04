// Copyright Epic Games, Inc. All Rights Reserved.


#include "DiamondProjectPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "UI/GameSettingsSubsystem.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>

class UGameSettingsSubsystem;


EKeyboardLayout ADiamondProjectPlayerController::GetCurrentlyAppliedKeyboardLayout() const
{
	return CurrentlyAppliedLayout;
}

void ADiamondProjectPlayerController::ChangeKeyboardLayout(EKeyboardLayout NewLayout)
{
	// Get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// Remove the old keyboard layout mapping contexts (priority 0)
		// We remove both to ensure we're starting clean
		if (QWERTYInputMappingContext)
		{
			Subsystem->RemoveMappingContext(QWERTYInputMappingContext);
		}
		if (AZERTYInputMappingContext)
		{
			Subsystem->RemoveMappingContext(AZERTYInputMappingContext);
		}

		// Add the new mapping context based on the layout
		switch (NewLayout)
		{
			case EKeyboardLayout::QWERTY:
				if (QWERTYInputMappingContext)
				{
					Subsystem->AddMappingContext(QWERTYInputMappingContext, 0);
					CurrentlyAppliedLayout = EKeyboardLayout::QWERTY;
				}
				break;
			case EKeyboardLayout::AZERTY:
				if (AZERTYInputMappingContext)
				{
					Subsystem->AddMappingContext(AZERTYInputMappingContext, 0);
					CurrentlyAppliedLayout = EKeyboardLayout::AZERTY;
				}
				break;
			default:
				// UNSPECIFIED or other values - do nothing
				CurrentlyAppliedLayout = EKeyboardLayout::UNSPECIFIED;
				break;
		}
	}
}

void ADiamondProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Get the game settings subsystem
	UGameSettingsSubsystem* GameSettings = GetGameInstance()->GetSubsystem<UGameSettingsSubsystem>();
	if (!GameSettings)
	{
		return;
	}

	EKeyboardLayout LayoutToApply = GameSettings->SavedKeyboardLayout;

	// If the saved layout is unspecified, detect it and save it
	if (LayoutToApply == EKeyboardLayout::UNSPECIFIED)
	{
		LayoutToApply = DetectKeyboardLayout();
		GameSettings->SavedKeyboardLayout = LayoutToApply;
		GameSettings->SaveSettings();
	}

	// Apply the keyboard layout
	ChangeKeyboardLayout(LayoutToApply);
}

EKeyboardLayout ADiamondProjectPlayerController::DetectKeyboardLayout()
{
#if PLATFORM_WINDOWS
	HKL KeyboardLayout = GetKeyboardLayout(0);
	WORD LangID = LOWORD(KeyboardLayout);

	switch (LangID)
	{
	case 0x040C: // French (France)
	case 0x080C: // French (Belgium)
		return EKeyboardLayout::AZERTY; // French-family layouts -> AZERTY

	case 0x0409: // English (United States)
	case 0x0809: // English (United Kingdom)
	case 0x0C09: // English (Australia)
	case 0x1009: // English (Canada)
	case 0x1409: // English (New Zealand)
	case 0x1C09: // English (Ireland)
	case 0x1809: // English (Hong Kong)
		return EKeyboardLayout::QWERTY; // Common English layouts -> QWERTY

	default:
		return EKeyboardLayout::AZERTY; // Default to AZERTY
	}
#else
	// On non-Windows platforms, default to AZERTY
	return EKeyboardLayout::AZERTY;
#endif
}
