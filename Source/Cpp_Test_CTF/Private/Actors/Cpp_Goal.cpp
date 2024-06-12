// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Cpp_Goal.h"

ACpp_Goal::ACpp_Goal() {
	PrimaryActorTick.bCanEverTick = false;

	// Create Goal Mesh
	GoalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Goal Mesh"));
	GoalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(GoalMesh);

	// Create Goal Collision
	GoalCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Goal Collision"));
	GoalCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GoalCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	GoalCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}


void ACpp_Goal::BeginPlay() {
	Super::BeginPlay();

	
}
