// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DiamondProjectGameMode.generated.h"

UCLASS(minimalapi)
class ADiamondProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADiamondProjectGameMode();
	virtual void BeginPlay() override;

private:
	void InitPreplan() const;
};



