#include "LDIngredients/ArenaAntiDestructionButton.h"
#include "Components/StaticMeshComponent.h"

AArenaAntiDestructionButton::AArenaAntiDestructionButton()
{
    PrimaryActorTick.bCanEverTick = true;

    ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
    RootComponent = ButtonMesh;
}

void AArenaAntiDestructionButton::BeginPlay()
{
    Super::BeginPlay();
    if (ButtonMesh && BaseMaterial) {
        ButtonMesh->SetMaterial(0, BaseMaterial);
    }
}

void AArenaAntiDestructionButton::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AArenaAntiDestructionButton::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    
    if (ButtonMesh && BaseMaterial) {
        ButtonMesh->SetMaterial(0, BaseMaterial);
    }
}

void AArenaAntiDestructionButton::OnButtonPressed()
{
    if (bIsActivated) {
        GetWorld()->GetTimerManager().ClearTimer(DestructionCountdownTimerHandle);
        
        if (BaseMaterial) {
            ButtonMesh->SetMaterial(0, BaseMaterial);
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("BaseMaterial is null!"));
        }
        
        bIsActivated = false;
        StartRandomActivationTimer();
    }
}

void AArenaAntiDestructionButton::StartRandomActivationTimer()
{
    float RandomTime = FMath::RandRange(2.0f, MaxDelayTime);
    GetWorld()->GetTimerManager().SetTimer(
        RandomActivationTimerHandle, 
        this, 
        &AArenaAntiDestructionButton::ActivateButton, 
        RandomTime, 
        false
    );
}

void AArenaAntiDestructionButton::StopDestruction()
{
    if (DestructionCountdownTimerHandle.IsValid()) {
        GetWorld()->GetTimerManager().ClearTimer(DestructionCountdownTimerHandle);
    }

    if (RandomActivationTimerHandle.IsValid()) {
        GetWorld()->GetTimerManager().ClearTimer(RandomActivationTimerHandle);
    }

    bIsActivated = false;
}

void AArenaAntiDestructionButton::StartFailureTimer()
{
    GetWorld()->GetTimerManager().SetTimer(
        DestructionCountdownTimerHandle, 
        this, 
        &AArenaAntiDestructionButton::DestroyArena, 
        TimeActive, 
        false
    );
}

void AArenaAntiDestructionButton::ResetAfterFailure()
{
    if (BaseMaterial) {
        ButtonMesh->SetMaterial(0, BaseMaterial);
    }
    
    StartRandomActivationTimer();
}

void AArenaAntiDestructionButton::ActivateButton()
{
    if (ActivatedMaterial) {
        ButtonMesh->SetMaterial(0, ActivatedMaterial);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("ActivatedMaterial is null!"));
    }
    
    StartFailureTimer();
    bIsActivated = true;
}

void AArenaAntiDestructionButton::DestroyArena()
{
    bIsActivated = false;

    UWorld* World = GetWorld();
    if (!World) return;
    
    float RandomXOffset = FMath::RandRange(-200.0f, 200.0f);
    float RandomZOffset = FMath::RandRange(-200.0f, 200.0f);
    
    FVector SpawnPosition = GetActorLocation() + FVector(RandomXOffset, 0, 1000.0f + RandomZOffset);
    FRotator SpawnRotation = GetActorRotation();
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    
    if (DestructionActorClass)
    {
        AActor* DestructionActor = World->SpawnActor<AActor>(
            DestructionActorClass,
            SpawnPosition,
            SpawnRotation,
            SpawnParams
        );
        
        if (DestructionActor)
        {
            UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(
                DestructionActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

            if (MeshComponent)
            {
                MeshComponent->SetSimulatePhysics(true);
                MeshComponent->AddImpulse(FVector(0, 0, -500.0f));
                
                float RandomX = FMath::RandRange(-10.0f, 10.0f);
                float RandomY = FMath::RandRange(-10.0f, 10.0f);
                float RandomZ = FMath::RandRange(-10.0f, 10.0f);
                MeshComponent->SetPhysicsAngularVelocityInDegrees(FVector(RandomX, RandomY, RandomZ));
            }
        }
    }
    
    if (FailMaterial) {
        ButtonMesh->SetMaterial(0, FailMaterial);
    }
    
    GetWorld()->GetTimerManager().SetTimer(
        FailMaterialDisplayTimerHandle, 
        this, 
        &AArenaAntiDestructionButton::ResetAfterFailure, 
        FailDuration, 
        false
    );
}
