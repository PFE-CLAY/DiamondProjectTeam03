// Fill out your copyright notice in the Description page of Project Settings.


#include "LDIngredients/JumpPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

AJumpPad::AJumpPad()
{
	PrimaryActorTick.bCanEverTick = true;
	
	JumpPadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JumpPadMesh"));
	RootComponent = JumpPadMesh;
	
	JumpCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("JumpCollider"));
	JumpCollider->SetupAttachment(JumpPadMesh);
	JumpCollider->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); 
	JumpCollider->SetBoxExtent(FVector(60.0f, 60.0f, 20.0f)); 
	
	JumpForce = 1000.0f;
	
	JumpCollider->OnComponentBeginOverlap.AddDynamic(this, &AJumpPad::OnOverlapBegin);
}

void AJumpPad::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!JumpPadMesh) return;
	JumpPadMesh->SetMaterial(0, BaseMaterial);
}

void AJumpPad::BeginPlay()
{
	Super::BeginPlay();
}

void AJumpPad::SetIsJumpadIlluminated(bool bNewValue)
{
	bIsIlluminated = bNewValue;
}

void AJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJumpPad::OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
 class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bCanBounce || !OtherActor) return;

	if (bDoesJumppadRequireSun){
		if (!bIsIlluminated) return;
	}

	FVector JumpVector = JumpPadMesh->GetUpVector() * JumpForce;

	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		Character->LaunchCharacter(JumpVector, true, true);
  
		// Apply cooldown only for characters
		JumpPadMesh->SetMaterial(0, CooldownMaterial);
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AJumpPad::ResetJump, CooldownDuration, false);
		bCanBounce = false;

		JumpadJump.Broadcast();
	}
	else if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(OtherComp))
	{
		if (PrimComp->IsSimulatingPhysics())
		{
			PrimComp->AddImpulse(JumpVector, NAME_None, true);
			// No cooldown for physics objects
		}
	}
}

void AJumpPad::ResetJump()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
	bCanBounce = true;
	JumpPadMesh->SetMaterial(0, BaseMaterial);
}

