#include "LDIngredients/MultiSunlightDetectorComponent.h"
#include "LDIngredients/SunlightTracePointComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"

UMultiSunlightDetectorComponent::UMultiSunlightDetectorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bPreviousIsActivatedBySun = false;
    
    TracePoints.Reserve(4);
}

void UMultiSunlightDetectorComponent::BeginPlay()
{
    Super::BeginPlay();
    TryAssignSunComponentFromActor();
    
    // Start the timer for periodic sunlight checks
    GetWorld()->GetTimerManager().SetTimer(
        SunActivationTimerHandle,
        this,
        &UMultiSunlightDetectorComponent::SunActivationTimerCallback,
        SunActivationTimerLength,
        true // Loop the timer
    );
}

void UMultiSunlightDetectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // Sunlight check moved to timer callback
}

void UMultiSunlightDetectorComponent::RegisterTracePoint(USunlightTracePointComponent* TracePoint)
{
    if (TracePoint && !TracePoints.Contains(TracePoint)) {
        TracePoints.Add(TracePoint);
    }
}

void UMultiSunlightDetectorComponent::UnregisterTracePoint(USunlightTracePointComponent* TracePoint)
{
    if (TracePoint) {
        TracePoints.Remove(TracePoint);
    }
}

void UMultiSunlightDetectorComponent::PerformSunlightCheck()
{
    if (TracePoints.Num() == 0) {
        if (bIsActivatedBySun) {
            bIsActivatedBySun = false;
            OnSunActivationChanged.Broadcast(false);
        }
        return;
    }

    if (!CachedSunComponent) {
        TryAssignSunComponentFromActor();
        if (!CachedSunComponent) {
            return;
        }
    }

    const FVector SunDirection = -CachedSunComponent->GetComponentRotation().Vector();
    int8 SunlitTraceCount = 0;
    const AActor* OwningActor = GetOwner();

    for (USunlightTracePointComponent* TracePoint : TracePoints) {
        if (!TracePoint)
            continue;

        const FVector Start = TracePoint->GetComponentLocation();
        const FVector End = Start + SunDirection * TraceDistance;

        FHitResult Hit;
        FCollisionQueryParams Params;
        if (OwningActor) {
            Params.AddIgnoredActor(OwningActor);
        }

        bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
        TracePoint->bIsInSunlight = !bHit;

        if (!bHit)
            SunlitTraceCount++;

        if (bShowDebugLines) {
            DrawDebugLine(
                GetWorld(),
                Start,
                bHit ? Hit.ImpactPoint : End,
                bHit ? FColor::Red : FColor::Green,
                false,
                0.0f,
                0,
                2.0f
            );
        }
    }
    
    const int8 MinPointsNeeded = FMath::Min(TracesNeededToActivate, TracePoints.Num());
    const bool bNewState = SunlitTraceCount >= MinPointsNeeded;
    
    if (bIsActivatedBySun != bNewState) {
        bIsActivatedBySun = bNewState;
        OnSunActivationChanged.Broadcast(bIsActivatedBySun);
    }
}

void UMultiSunlightDetectorComponent::TryAssignSunComponentFromActor()
{
    if (CachedSunComponent || !SunActor.IsValid())
        return;

    AActor* Actor = SunActor.Get();
    if (!Actor) 
        return;
    
    CachedSunComponent = Actor->FindComponentByClass<UDirectionalLightComponent>();
}

void UMultiSunlightDetectorComponent::SunActivationTimerCallback()
{
    if (TracePoints.Num() > 0 || bIsActivatedBySun) {
        PerformSunlightCheck();
    }
}
