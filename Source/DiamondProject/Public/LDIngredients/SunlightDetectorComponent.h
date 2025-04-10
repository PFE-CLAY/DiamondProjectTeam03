#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "SunlightDetectorComponent.generated.h"

class UDirectionalLightComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIAMONDPROJECT_API USunlightDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USunlightDetectorComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category="Sunlight Detection")
	TSoftObjectPtr<AActor> SunActor;

	UPROPERTY(BlueprintReadOnly, Category="Sunlight Detection")
	bool bIsInSunlight = false;

	UPROPERTY(EditAnywhere, Category="Sunlight Detection")
	float TraceDistance = 100000.0f;

	UFUNCTION(BlueprintCallable, Category="Sunlight Detection")
	bool CheckSunlight();

private:
	UPROPERTY()
	UDirectionalLightComponent* CachedSunComponent = nullptr;

	void PerformSunlightCheck();
	void TryAssignSunComponentFromActor();
};
