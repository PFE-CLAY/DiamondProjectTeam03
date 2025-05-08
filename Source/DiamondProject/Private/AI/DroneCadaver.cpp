// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DroneCadaver.h"

#include "AI/Allied.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ADroneCadaver::ADroneCadaver()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(Root);
	GrabPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GrabPoint"));
	GrabPoint->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void ADroneCadaver::BeginPlay()
{
	Super::BeginPlay();
	HatchIncinerator = Cast<AHatchIncinerator>(UGameplayStatics::GetActorOfClass(GetWorld(), AHatchIncinerator::StaticClass()));
	if(HatchIncinerator == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Red, "No Hatch in scene");
	}
	
	TArray<AActor*> AlliedActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAllied::StaticClass(), AlliedActors);
	for (auto AlliedActor : AlliedActors)
	{
		AllAllies.Add(Cast<AAllied>(AlliedActor));	
	}
}

// Called every frame
void ADroneCadaver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(TargetBody != nullptr && bShouldGrab){
		UpdateTargetLocation();
	}
	
}

// Called to bind functionality to input
void ADroneCadaver::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void ADroneCadaver::GoToNewTarget()
{
	if(CadaverArray.IsEmpty())
	{
		return;
	}
	TargetBody = CadaverArray[0];
	CadaverArray.Remove(TargetBody);
	OnMovingToNewCadaver(TargetBody);
}

void ADroneCadaver::CarryBody()
{
	AActor* OwnedActor = GetController()->GetPawn();
	TargetBody->PhysicsHandle->GrabComponentAtLocation(TargetBody->AlliedMesh, TargetBody->GrabBoneName,
		TargetBody->AlliedMesh->GetBoneLocation(TargetBody->GrabBoneName));
	//TODO: ne collisione plus 
	bShouldGrab = true;
}

void ADroneCadaver::ThrowBody()
{
	TargetBody->PhysicsHandle->ReleaseComponent();
	bShouldGrab = false;
	if(CadaverArray.IsEmpty())
	{
		GoInHatch();
	}
	else
	{
		GoToNewTarget();
	}
	
	
}

void ADroneCadaver::UpdateTargetLocation()
{
	TargetBody->PhysicsHandle->SetTargetLocation(GetActorLocation());
}

void ADroneCadaver::GetNewCadaver(AAllied* Allied)
{
	CadaverArray.Add(Allied);
	if(CadaverArray.Num() == 1)
	{
		GoOutHatch();
	}
}




