#include "LDIngredients/SunlightDetectorComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Components/DirectionalLightComponent.h"

USunlightDetectorComponent::USunlightDetectorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USunlightDetectorComponent::BeginPlay()
{
    Super::BeginPlay();
    TryAssignSunComponentFromActor();
}

void USunlightDetectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    PerformSunlightCheck();
}

void USunlightDetectorComponent::PerformSunlightCheck()
{
    bIsInSunlight = CheckSunlight();
}

bool USunlightDetectorComponent::CheckSunlight()
{
    if (!CachedSunComponent)
    {
        //Debug message if the sun component is not assigned
        UE_LOG(LogTemp, Warning, TEXT("SunlightDetectorComponent: Sun component not assigned. Attempting to assign..."));
        return false;
    }

    FVector SunDirection = -CachedSunComponent->GetComponentRotation().Vector();
    FVector Start = GetOwner()->GetActorLocation();
    FVector End = Start + SunDirection * TraceDistance;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetOwner());

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    FColor LineColor = bHit ? FColor::Red : FColor::Green;
    float DebugLifeTime = 0.0f; // Set to 0 for single frame or higher for persistence
    float Thickness = 2.0f;
        
    DrawDebugLine(
        GetWorld(),
        Start,
        bHit ? Hit.ImpactPoint : End,
        LineColor,
        false,
        DebugLifeTime,
        0,
        Thickness
    );
    
    return !bHit;
}

void USunlightDetectorComponent::TryAssignSunComponentFromActor()
{
    if (!SunActor.IsValid()){
        //Debug message if the sun actor is not valid
        UE_LOG(LogTemp, Warning, TEXT("SunlightDetectorComponent: Sun actor is not valid."));
        return;
    } 

    AActor* Actor = SunActor.Get();
    if (!Actor) return;

    // Alternative approach using GetComponents with StaticClass
    TArray<UActorComponent*> Components;
    Actor->GetComponents(UDirectionalLightComponent::StaticClass(), Components);
    if (Components.Num() > 0)
    {
        CachedSunComponent = Cast<UDirectionalLightComponent>(Components[0]);
    }
}