// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ITriggerable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UTriggerable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIAMONDPROJECT_API ITriggerable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Fonction virtuelle pouvant être implémentée dans un Blueprint
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Custom Event")
	void OnTriggerEvent();
};
