// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "JumpPad.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJumpadJump);

UCLASS()
class DIAMONDPROJECT_API AJumpPad : public AActor
{
	GENERATED_BODY()

public:
	AJumpPad();
	virtual void OnConstruction(const FTransform& Transform) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)
	FOnJumpadJump JumpadJump;

	UPROPERTY(EditAnywhere)
	bool bDoesJumppadRequireSun = false;

	UFUNCTION(BlueprintCallable)
	void SetIsJumpadIlluminated(bool bIsIlluminated);

public:
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* JumpPadMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* JumpCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JumpValues")
	float JumpForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JumpValues")
	float CooldownDuration = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInstance* CooldownMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInstance* BaseMaterial;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private :
	
	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY()
	bool bCanBounce = true;

	bool bIsIlluminated = false;
	
	void ResetJump();
};