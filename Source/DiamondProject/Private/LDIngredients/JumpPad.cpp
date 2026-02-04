// Fill out your copyright notice in the Description page of Project Settings.


#include "LDIngredients/JumpPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "AI/Enemy.h"

AJumpPad::AJumpPad()
{
	PrimaryActorTick.bCanEverTick = true;
	
	TObjectPtr<USceneComponent> DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = DefaultRootComponent;

	JumpPadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JumpPadMesh"));
	JumpPadMesh->SetupAttachment(DefaultRootComponent);
	
	
	JumpCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("JumpCollider"));
	JumpCollider->SetupAttachment(DefaultRootComponent);
	JumpCollider->SetWorldLocation(JumpPadMesh->GetComponentLocation()); 
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

// Default launch vector uses the actor's up vector scaled by JumpForce
FVector AJumpPad::ComputeJumpVector() const
{
	return (JumpPadMesh ? JumpPadMesh->GetUpVector() : GetActorUpVector()) * JumpForce;
}

// Default gating allows trigger; sun requirement is enforced here so subclasses inherit it
bool AJumpPad::ShouldTriggerOnOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, bool bFromSweep, const FHitResult& SweepResult) const
{
	if (bDoesJumppadRequireSun && !bIsIlluminated)
	{
		return false;
	}
	return true;
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

	if (!ShouldTriggerOnOverlap(OtherActor, OtherComp, bFromSweep, SweepResult))
	{
		return;
	}

	const FVector JumpVector = ComputeJumpVector();

	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		Character->LaunchCharacter(JumpVector, true, true);
		
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AJumpPad::ResetJump, CooldownDuration, false);
		bCanBounce = false;

		JumpadJump.Broadcast();

		if (Cast<AEnemy>(OtherActor))
		{
			JumpadEnemyLaunched.Broadcast();
		}
	}
	else if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(OtherComp))
	{
		if (PrimComp->IsSimulatingPhysics())
		{
			PrimComp->AddImpulse(JumpVector, NAME_None, true);
		}
	}
}

void AJumpPad::ResetJump()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
	bCanBounce = true;
}
