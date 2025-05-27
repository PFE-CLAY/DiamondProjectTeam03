// Fill out your copyright notice in the Description page of Project Settings.


#include "WwiseManagerSubsystem/Commentator.h"

#include "CommentarySubSystem/CommentarySubSystem.h"


// Sets default values
ACommentator::ACommentator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ACommentator::BeginPlay()
{
	Super::BeginPlay();
	GetGameInstance()->GetSubsystem<UCommentarySubSystem>()->RegisterActor(this, InterruptEvent);
}

// Called every frame
void ACommentator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

