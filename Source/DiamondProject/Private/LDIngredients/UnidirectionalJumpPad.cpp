#include "LDIngredients/UnidirectionalJumpPad.h"
#include "Components/ArrowComponent.h"

AUnidirectionalJumpPad::AUnidirectionalJumpPad()
{
	DirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionArrow"));
	DirectionArrow->SetupAttachment(RootComponent);
	DirectionArrow->SetArrowColor(FLinearColor::Green);
	DirectionArrow->ArrowSize = 1.5f;
}

void AUnidirectionalJumpPad::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

FVector AUnidirectionalJumpPad::ComputeJumpVector() const
{
	const FVector Direction = DirectionArrow ? DirectionArrow->GetForwardVector() : GetActorForwardVector();
	return Direction.GetSafeNormal() * JumpForce;
}
