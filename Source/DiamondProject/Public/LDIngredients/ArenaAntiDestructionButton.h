#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaAntiDestructionButton.generated.h"

/**
 * A button that spawns a destruction actor after activation if not pressed in time
 */
UCLASS()
class DIAMONDPROJECT_API AArenaAntiDestructionButton : public AActor
{
    GENERATED_BODY()

public:
    AArenaAntiDestructionButton();
    
    virtual void Tick(float DeltaTime) override;
    
    UFUNCTION(BlueprintCallable, Category = "Button")
    void OnButtonPressed();
    
    UFUNCTION(BlueprintCallable, Category = "Button")
    void StartRandomActivationTimer();
    
    UFUNCTION(BlueprintCallable, Category = "Button")
    void StopDestruction();
    
protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;
    
    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* ButtonMesh;
    
    // Materials
    UPROPERTY(EditAnywhere, Category = "Materials")
    UMaterialInstance* BaseMaterial;
    
    UPROPERTY(EditAnywhere, Category = "Materials")
    UMaterialInstance* ActivatedMaterial;
    
    UPROPERTY(EditAnywhere, Category = "Materials")
    UMaterialInterface* FailMaterial;
    
    // Configuration
    UPROPERTY(EditAnywhere, Category = "Timing", meta = (UIMin = "0", UIMax = "100"))
    float MaxDelayTime = 10.0f;
    
    UPROPERTY(EditAnywhere, Category = "Timing", meta = (ClampMin = "0.1"))
    float FailDuration = 3.0f;
    
    UPROPERTY(EditAnywhere, Category = "Timing", meta = (ClampMin = "0.1"))
    float TimeActive = 5.0f;
    
    UPROPERTY(EditDefaultsOnly, Category = "Arena Destruction")
    TSubclassOf<AActor> DestructionActorClass;
    
    // State
    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bIsActivated = false;
    
private:
    // Timer handles
    UPROPERTY()
    FTimerHandle RandomActivationTimerHandle;
    
    UPROPERTY()
    FTimerHandle DestructionCountdownTimerHandle;
    
    UPROPERTY()
    FTimerHandle FailMaterialDisplayTimerHandle;
    
    // Button state methods
    void ActivateButton();
    void DestroyArena();
    void StartFailureTimer();
    void ResetAfterFailure();
};