// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiamondProjectCharacter.h"
#include "DiamondProjectProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ADiamondProjectCharacter

ADiamondProjectCharacter::ADiamondProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void ADiamondProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////// Input

void ADiamondProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADiamondProjectCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADiamondProjectCharacter::Look);

		//Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ADiamondProjectCharacter::Interact);

		//Mantling
		EnhancedInputComponent->BindAction(MantleAction, ETriggerEvent::Started, this, &ADiamondProjectCharacter::Mantle);

		//Pause
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ADiamondProjectCharacter::PressPause);

		//Preplan Move
		EnhancedInputComponent->BindAction(PreplanMoveAction, ETriggerEvent::Triggered, this, &ADiamondProjectCharacter::PreplanMove);

		//Preplan Zoom
		EnhancedInputComponent->BindAction(PreplanZoomAction, ETriggerEvent::Triggered, this, &ADiamondProjectCharacter::PreplanZoom);

		// Cheat actions
		EnhancedInputComponent->BindAction(CheatOpenHatchAction, ETriggerEvent::Started, this, &ADiamondProjectCharacter::CheatOpenHatch);
		
		EnhancedInputComponent->BindAction(CheatOpenPyramidAction, ETriggerEvent::Started, this, &ADiamondProjectCharacter::CheatOpenPyramid);
		
		EnhancedInputComponent->BindAction(CheatTogglePlayerInvincibilityAction, ETriggerEvent::Started, this, &ADiamondProjectCharacter::CheatTogglePlayerInvincibility);
		
		EnhancedInputComponent->BindAction(CheatToggleAllyInvincibilityAction, ETriggerEvent::Started, this, &ADiamondProjectCharacter::CheatToggleAllyInvincibility);
		
		EnhancedInputComponent->BindAction(CheatEndLoopAction, ETriggerEvent::Started, this, &ADiamondProjectCharacter::CheatEndLoop);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ADiamondProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ADiamondProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADiamondProjectCharacter::Interact(const FInputActionValue& Value)
{
	OnInteract.Broadcast();
}

void ADiamondProjectCharacter::Mantle(const FInputActionValue& Value)
{
	OnMantle.Broadcast();
}

void ADiamondProjectCharacter::PressPause(const FInputActionValue& Value)
{
	OnPause.Broadcast();
}

void ADiamondProjectCharacter::PreplanMove(const FInputActionValue& Value)
{
	FVector2D PreplanMovementVector = Value.Get<FVector2D>();
	OnPreplanMove.Broadcast(PreplanMovementVector);
}

void ADiamondProjectCharacter::PreplanZoom(const FInputActionValue& Value)
{
	float PreplanZoomValue = Value.Get<float>();
	OnPreplanZoom.Broadcast(PreplanZoomValue);
}

void ADiamondProjectCharacter::CheatOpenHatch(const FInputActionValue& Value)
{
	OnCheatOpenHatch.Broadcast();
}

void ADiamondProjectCharacter::CheatOpenPyramid(const FInputActionValue& Value)
{
	OnCheatPyramidOpen.Broadcast();
}

void ADiamondProjectCharacter::CheatTogglePlayerInvincibility(const FInputActionValue& Value)
{
	OnCheatTogglePlayerInvincibility.Broadcast();
}

void ADiamondProjectCharacter::CheatToggleAllyInvincibility(const FInputActionValue& Value)
{
	OnCheatToggleAllyInvincibility.Broadcast();
}

void ADiamondProjectCharacter::CheatEndLoop(const FInputActionValue& Value)
{
	OnCheatEndLoop.Broadcast();
}
