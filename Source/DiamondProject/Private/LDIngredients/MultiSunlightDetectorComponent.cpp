// MultiSunlightDetectorComponent.cpp
#include "LDIngredients/MultiSunlightDetectorComponent.h"
#include "LDIngredients/SunlightTracePointComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"

UMultiSunlightDetectorComponent::UMultiSunlightDetectorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bPreviousIsActivatedBySun = false;
}

void UMultiSunlightDetectorComponent::BeginPlay()
{
    Super::BeginPlay();
    TryAssignSunComponentFromActor();
    // Trace points register themselves automatically when created
}

void UMultiSunlightDetectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    PerformSunlightCheck();
}

void UMultiSunlightDetectorComponent::RegisterTracePoint(USunlightTracePointComponent* TracePoint)
{
    if (TracePoint && !TracePoints.Contains(TracePoint)){
        TracePoints.Add(TracePoint);
        UE_LOG(LogTemp, Verbose, TEXT("MultiSunlightDetectorComponent: Registered trace point. Total: %d"), TracePoints.Num());
    }
}

void UMultiSunlightDetectorComponent::UnregisterTracePoint(USunlightTracePointComponent* TracePoint)
{
    if (TracePoint){
        int32 RemovedCount = TracePoints.Remove(TracePoint);
        if (RemovedCount > 0){
            UE_LOG(LogTemp, Verbose, TEXT("MultiSunlightDetectorComponent: Unregistered trace point. Remaining: %d"), TracePoints.Num());
        }
    }
}

void UMultiSunlightDetectorComponent::PerformSunlightCheck()
{
    if (TracePoints.Num() == 0){
        bool bNewState = false;

        if (bIsActivatedBySun != bNewState)
        {
            bIsActivatedBySun = bNewState;
            OnSunActivationChanged.Broadcast(bIsActivatedBySun);
        }
        return;
    }

    if (!CachedSunComponent){
        TryAssignSunComponentFromActor();
        if (!CachedSunComponent){
            UE_LOG(LogTemp, Warning, TEXT("MultiSunlightDetectorComponent: Sun component not assigned."));
            return;
        }
    }

    FVector SunDirection = -CachedSunComponent->GetComponentRotation().Vector();
    int32 SunlitTraceCount = 0;

    for (USunlightTracePointComponent* TracePoint : TracePoints)
    {
        if (!TracePoint)
            continue;

        FVector Start = TracePoint->GetComponentLocation();
        FVector End = Start + SunDirection * TraceDistance;

        FHitResult Hit;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(GetOwner());

        bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
        TracePoint->bIsInSunlight = !bHit;

        if (!bHit)
            SunlitTraceCount++;

        if (bShowDebugLines){
            FColor LineColor = bHit ? FColor::Red : FColor::Green;
            DrawDebugLine(
                GetWorld(),
                Start,
                bHit ? Hit.ImpactPoint : End,
                LineColor,
                false,
                0.0f,
                0,
                2.0f
            );
        }
    }

    // Update activation state and fire event if changed
    bool bNewState = SunlitTraceCount >= FMath::Min(TracesNeededToActivate, TracePoints.Num());
    if (bIsActivatedBySun != bNewState){
        bIsActivatedBySun = bNewState;
        OnSunActivationChanged.Broadcast(bIsActivatedBySun);
    }
}

void UMultiSunlightDetectorComponent::TryAssignSunComponentFromActor()
{
    if (!SunActor.IsValid()){
        UE_LOG(LogTemp, Warning, TEXT("MultiSunlightDetectorComponent: Sun actor is not valid."));
        return;
    }

    AActor* Actor = SunActor.Get();
    if (!Actor) return;

    TArray<UActorComponent*> Components;
    Actor->GetComponents(UDirectionalLightComponent::StaticClass(), Components);
    if (Components.Num() > 0){
        CachedSunComponent = Cast<UDirectionalLightComponent>(Components[0]);
    }
}