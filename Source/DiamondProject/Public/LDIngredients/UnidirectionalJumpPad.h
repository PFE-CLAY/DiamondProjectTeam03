// UnidirectionalJumpPad.h
#pragma once

#include "CoreMinimal.h"
#include "LDIngredients/JumpPad.h"
#include "Components/ArrowComponent.h"
#include "UnidirectionalJumpPad.generated.h"

class UArrowComponent;

UCLASS(meta=(DisplayName="Unidirectional Jump Pad"))
class DIAMONDPROJECT_API AUnidirectionalJumpPad : public AJumpPad
{
	GENERATED_BODY()
public:
	AUnidirectionalJumpPad();
	virtual void OnConstruction(const FTransform& Transform) override;
protected:
	virtual FVector ComputeJumpVector() const override;
	virtual bool ShouldTriggerOnOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, bool bFromSweep, const FHitResult& SweepResult) const override;

private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	UArrowComponent* DirectionArrow;

	UPROPERTY(EditAnywhere, Category="Unidirectional")
	bool bEnforceForwardOnly = true;

	// Cosine of the max angle allowed between pad forward and vector from pad to actor (0 deg = 1.0, 90 deg = 0)
	UPROPERTY(EditAnywhere, meta=(ClampMin="-1.0", ClampMax="1.0"), Category="Unidirectional")
	float MinForwardDotThreshold = 0.0f; // allow from front hemisphere by default
};

inline AUnidirectionalJumpPad::AUnidirectionalJumpPad()
{
	DirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionArrow"));
	DirectionArrow->SetupAttachment(RootComponent);
	DirectionArrow->SetArrowColor(FLinearColor::Green);
	DirectionArrow->ArrowSize = 1.5f;
}

inline void AUnidirectionalJumpPad::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

inline FVector AUnidirectionalJumpPad::ComputeJumpVector() const
{
	const FVector Dir = DirectionArrow ? DirectionArrow->GetForwardVector() : GetActorForwardVector();
	return Dir.GetSafeNormal() * JumpForce;
}

inline bool AUnidirectionalJumpPad::ShouldTriggerOnOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, bool bFromSweep, const FHitResult& SweepResult) const
{
	if (!Super::ShouldTriggerOnOverlap(OtherActor, OtherComp, bFromSweep, SweepResult))
	{
		return false;
	}

	if (!bEnforceForwardOnly || !DirectionArrow || !OtherActor)
	{
		return true;
	}

	const FVector Forward = DirectionArrow->GetForwardVector().GetSafeNormal();
	const FVector FromPadToActor = (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	const float Dot = FVector::DotProduct(Forward, FromPadToActor);
	return Dot >= MinForwardDotThreshold;
}
