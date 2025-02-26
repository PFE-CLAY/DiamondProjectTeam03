// Fill out your copyright notice in the Description page of Project Settings.


#include "LoopSystem/LoopSubsystem.h"

#include "Kismet/GameplayStatics.h"

void ULoopSubsystem::ReloadScene()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
