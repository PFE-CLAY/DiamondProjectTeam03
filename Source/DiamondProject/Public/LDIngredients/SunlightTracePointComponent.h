#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SunlightTracePointComponent.generated.h"

class UMultiSunlightDetectorComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIAMONDPROJECT_API USunlightTracePointComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	USunlightTracePointComponent();

	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Sunlight Detection")
	bool bShowVisualizer = true;

	UPROPERTY()
	bool bIsInSunlight = false;

	UFUNCTION(BlueprintCallable, Category="Sunlight Detection")
	void SetShowVisualizer(bool bNewState);

private:
	UPROPERTY()
	UStaticMeshComponent* VisualizerMesh = nullptr;

	UPROPERTY()
	UMultiSunlightDetectorComponent* CachedDetectorComponent = nullptr;

	void FindAndRegisterWithDetector();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void CreateVisualizerMesh();
};