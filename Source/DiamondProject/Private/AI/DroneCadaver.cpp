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
	UpdateTargetLocation();
}

// Called to bind functionality to input
void ADroneCadaver::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

AActor* ADroneCadaver::GetDeadBody()
{
	CarriedBody = nullptr;
	for (auto Allied : AllAllies){
		if(Allied->bIsDead){
			CarriedBody = Allied;
			break;
		}
	}
	return CarriedBody;
}

void ADroneCadaver::CarryBody()
{
	AActor* OwnedActor = GetController()->GetPawn();
	CarriedBody->PhysicsHandle->GrabComponentAtLocation(CarriedBody->AlliedMesh, CarriedBody->GrabBoneName,
		CarriedBody->AlliedMesh->GetBoneLocation(CarriedBody->GrabBoneName));
	//TODO: ne collision plus 
	//CarriedBody->SetActorLocation(FVector(0,0 ,0));
	//CarriedBody->AlliedMesh->AttachToComponent(GrabPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//CarriedBody->AttachToActor(OwnedActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	bShouldGrab = true;
}

void ADroneCadaver::UpdateTargetLocation()
{
	if(CarriedBody != nullptr && bShouldGrab){
		CarriedBody->PhysicsHandle->SetTargetLocation(GetActorLocation());
		
	}
	
}

