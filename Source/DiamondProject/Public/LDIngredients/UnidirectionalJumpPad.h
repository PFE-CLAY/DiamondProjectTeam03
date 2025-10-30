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

private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	UArrowComponent* DirectionArrow;
};

