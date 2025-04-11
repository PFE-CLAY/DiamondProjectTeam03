// SunlightTracePointComponent.cpp
#include "LDIngredients/SunlightTracePointComponent.h"
#include "LDIngredients/MultiSunlightDetectorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"

USunlightTracePointComponent::USunlightTracePointComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USunlightTracePointComponent::OnRegister()
{
    Super::OnRegister();
    
    CreateVisualizerMesh();
    
    if (GetWorld() && !GetWorld()->IsGameWorld())
    {
        FindAndRegisterWithDetector();
    }
}

void USunlightTracePointComponent::CreateVisualizerMesh()
{
    if (VisualizerMesh || IsRunningCommandlet())
        return;
    
    VisualizerMesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(),
        TEXT("VisualizerMesh"), RF_Transactional);
        
    if (!VisualizerMesh)
        return;
        
    VisualizerMesh->SetupAttachment(this);
    
    UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh)
    {
        VisualizerMesh->SetStaticMesh(CubeMesh);
    }
    
    VisualizerMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
    VisualizerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    VisualizerMesh->RegisterComponent();
}

void USunlightTracePointComponent::OnUnregister()
{
    if (CachedDetectorComponent)
    {
        CachedDetectorComponent->UnregisterTracePoint(this);
        CachedDetectorComponent = nullptr;
    }

    Super::OnUnregister();
}

void USunlightTracePointComponent::BeginPlay()
{
    Super::BeginPlay();
    
    if (VisualizerMesh)
    {
        VisualizerMesh->SetHiddenInGame(!bShowVisualizer);
    }
    
    FindAndRegisterWithDetector();
}

void USunlightTracePointComponent::FindAndRegisterWithDetector()
{
    if (!GetOwner())
        return;
    
    TArray<UMultiSunlightDetectorComponent*> DetectorComponents;
    GetOwner()->GetComponents<UMultiSunlightDetectorComponent>(DetectorComponents);

    if (DetectorComponents.Num() > 0)
    {
        CachedDetectorComponent = DetectorComponents[0];
        CachedDetectorComponent->RegisterTracePoint(this);
    }
}