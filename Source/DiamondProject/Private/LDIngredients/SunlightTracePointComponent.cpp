#include "LDIngredients/SunlightTracePointComponent.h"
#include "LDIngredients/MultiSunlightDetectorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Actor.h"

static UStaticMesh* CachedCubeMesh = nullptr;

USunlightTracePointComponent::USunlightTracePointComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USunlightTracePointComponent::OnRegister()
{
    Super::OnRegister();
    
    if (bShowVisualizer) {
        CreateVisualizerMesh();
    }
    
    if (GetWorld() && !GetWorld()->IsGameWorld()) {
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
    
    if (!CachedCubeMesh) {
        CachedCubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
    }
    
    VisualizerMesh->SetStaticMesh(CachedCubeMesh);

    
    VisualizerMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
    VisualizerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    VisualizerMesh->RegisterComponent();
}

void USunlightTracePointComponent::OnUnregister()
{
    if (CachedDetectorComponent) {
        CachedDetectorComponent->UnregisterTracePoint(this);
        CachedDetectorComponent = nullptr;
    }

    CachedCubeMesh = nullptr;
    
    Super::OnUnregister();
}

void USunlightTracePointComponent::BeginPlay()
{
    Super::BeginPlay();
    
    if (VisualizerMesh) {
        VisualizerMesh->SetHiddenInGame(!bShowVisualizer);
    }
    
    FindAndRegisterWithDetector();
}

void USunlightTracePointComponent::FindAndRegisterWithDetector()
{
    const AActor* Owner = GetOwner();
    if (!Owner)
        return;
    

    if (UMultiSunlightDetectorComponent* DetectorComponent = Owner->FindComponentByClass<UMultiSunlightDetectorComponent>()) {
        CachedDetectorComponent = DetectorComponent;
        CachedDetectorComponent->RegisterTracePoint(this);
    }
}