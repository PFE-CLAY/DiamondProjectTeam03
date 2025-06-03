// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TP_WeaponComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DiamondProjectCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMantle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPause);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPreplanMove, FVector2D, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPreplanZoom, float, ZoomValue);

UCLASS(Blueprintable,config=Game)
class ADiamondProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MantleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* PreplanMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* PreplanZoomAction;
	
public:
	ADiamondProjectCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteract OnInteract;
	
	
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnMantle OnMantle;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnPause OnPause;

	UPROPERTY(BlueprintAssignable, Category = "Preplan")
	FOnPreplanMove OnPreplanMove;
	
	UPROPERTY(BlueprintAssignable, Category = "Preplan")
	FOnPreplanZoom OnPreplanZoom;
	
	UPROPERTY()
	TObjectPtr<UTP_WeaponComponent> CurrentWeapon;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);
	
	void Mantle(const FInputActionValue& Value);

	void PressPause(const FInputActionValue& Value);

	void PreplanMove(const FInputActionValue& Value);

	void PreplanZoom(const FInputActionValue& Value);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	

};