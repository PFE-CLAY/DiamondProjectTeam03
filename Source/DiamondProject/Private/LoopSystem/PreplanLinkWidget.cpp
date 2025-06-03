// Fill out your copyright notice in the Description page of Project Settings.


#include "LoopSystem/PreplanLinkWidget.h"

bool UPreplanLinkWidget::IsLinkActive() const
{
	return bOutDataActive && bInDataActive;
}

void UPreplanLinkWidget::ActivateFromData()
{
	bInDataActive = true;
}

void UPreplanLinkWidget::ActivateToData()
{
	bOutDataActive = true;
}

void UPreplanLinkWidget::ChangeVisibility(bool bIsVisible)
{
	if (bIsVisible)
	{
		SetVisibility(ESlateVisibility::Visible);
	} else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}
