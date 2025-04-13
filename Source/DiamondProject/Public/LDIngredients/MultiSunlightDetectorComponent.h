#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "MultiSunlightDetectorComponent.generated.h"

class USunlightTracePointComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSunActivationChangedDelegate, bool, IsActivated);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIAMONDPROJECT_API UMultiSunlightDetectorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMultiSunlightDetectorComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, Category="Sunlight Detection")
    TSoftObjectPtr<AActor> SunActor;
    
    UPROPERTY(EditAnywhere, Category="Sunlight Detection")
    bool bShowDebugLines = true;
    
    UPROPERTY(EditAnywhere, Category="Sunlight Detection")
    float TraceDistance = 100000.0f;
    
    UPROPERTY(EditAnywhere, Category="Sunlight Detection", meta=(ClampMin="1", UIMin="1"))
    int8 TracesNeededToActivate = 1;
    
    UPROPERTY(BlueprintReadOnly, Category="Sunlight Detection")
    bool bIsActivatedBySun = false;
    
    UPROPERTY(BlueprintAssignable, Category="Sunlight Detection")
    FSunActivationChangedDelegate OnSunActivationChanged;
    
    void RegisterTracePoint(USunlightTracePointComponent* TracePoint);
    
    void UnregisterTracePoint(USunlightTracePointComponent* TracePoint);

private:
    UPROPERTY()
    UDirectionalLightComponent* CachedSunComponent = nullptr;

    UPROPERTY()
    TArray<USunlightTracePointComponent*> TracePoints;

    void PerformSunlightCheck();
    void TryAssignSunComponentFromActor();
    
    bool bPreviousIsActivatedBySun = false;
};