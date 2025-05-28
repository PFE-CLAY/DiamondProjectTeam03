// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EPointType : uint8 {
	None UMETA(DisplayName = "None"),
	Crouch UMETA(DisplayName = "Crouch"),
	Choice UMETA(DisplayName = "Choice")
};
