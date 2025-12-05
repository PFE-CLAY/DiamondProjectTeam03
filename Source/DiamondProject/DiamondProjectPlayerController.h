// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DiamondProjectPlayerController.generated.h"

class UInputMappingContext;

UENUM(BlueprintType)
enum class EKeyboardLayout : uint8
{
	QWERTY,
	AZERTY,
	UNSPECIFIED
};


/**
 *
 */
UCLASS()
class DIAMONDPROJECT_API ADiamondProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* AZERTYInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* QWERTYInputMappingContext;

private:
	/** Currently applied keyboard layout */
	UPROPERTY()
	EKeyboardLayout CurrentlyAppliedLayout = EKeyboardLayout::UNSPECIFIED;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Input)
	EKeyboardLayout GetCurrentlyAppliedKeyboardLayout() const;
	
protected:
	UFUNCTION(BlueprintCallable, Category = Input)
	void ChangeKeyboardLayout(EKeyboardLayout NewLayout);

	// Begin Actor interface
protected:
	virtual void BeginPlay() override;
	EKeyboardLayout DetectKeyboardLayout();
	// End Actor interface
};

