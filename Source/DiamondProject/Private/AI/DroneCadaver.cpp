// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DroneCadaver.h"

#include "AI/Allied.h"
#include "Components/CapsuleComponent.h"
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
	FVector Location = TargetBody->AlliedMesh->GetBoneLocation(TargetBody->GrabBoneName);
	TargetBody->PhysicsHandle->GrabComponentAtLocation(TargetBody->AlliedMesh, TargetBody->GrabBoneName,
		Location);
	TargetBody->AlliedMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
	TargetBody->AlliedMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	TargetBody->CapsuleAllied->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bShouldGrab = true;
}

void ADroneCadaver::ThrowBody()
{
	TargetBody->PhysicsHandle->ReleaseComponent();
	TargetBody->AlliedMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	TargetBody->AlliedMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	TargetBody->CapsuleAllied->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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




