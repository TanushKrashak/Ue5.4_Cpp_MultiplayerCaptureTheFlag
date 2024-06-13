// Fill out your copyright notice in the Description page of Project Settings.

// Game Includes
#include "Actors/Cpp_Goal.h"
#include "Interfaces/Cpp_InteractionInterface.h"
#include "Actors/Cpp_Flag.h"

// Engine Includes
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"


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
	GoalCollision->OnComponentBeginOverlap.AddDynamic(this, &ACpp_Goal::OnGoalOverlap);
	GoalCollision->SetupAttachment(GoalMesh);
	GoalCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	GoalCollision->SetBoxExtent(FVector(250.0f, 150.0f, 150.0f));
}


void ACpp_Goal::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	if (bIsTeamA) {
		GoalMesh->SetMaterial(0, TeamAMaterial);
	}
	else {
		GoalMesh->SetMaterial(0, TeamBMaterial);
	}
}
void ACpp_Goal::BeginPlay() {
	Super::BeginPlay();

	// Add possible spawn locations for flags, this method is less resource intensive than using Actors
	FlagSpawnLocations.Empty();
	FlagSpawnLocations.Add(FVector(2940.0f, 3560.0f, 40.0f));
	FlagSpawnLocations.Add(FVector(2940.0f, 2570.0f, 40.0f));
	FlagSpawnLocations.Add(FVector(2940.0f, 1080.0f, 40.0f));
	FlagSpawnLocations.Add(FVector(2100.0f, 1080.0f, 40.0f));
	FlagSpawnLocations.Add(FVector(2100.0f, 4610.0f, 40.0f));
	FlagSpawnLocationsLength = int(FlagSpawnLocations.Num());
}

void ACpp_Goal::OnGoalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (HasAuthority()) {
		MC_OnGoalOverlap(OtherActor);
	}
}

void ACpp_Goal::MC_OnGoalOverlap_Implementation(AActor* OtherActor) {	
	// check if other actor implements the interaction interface
	ICpp_InteractionInterface* Interactable = Cast<ICpp_InteractionInterface>(OtherActor);
	// if it does, check if it is on the opposite team
	if (Interactable && Interactable->GetIsTeamA() != bIsTeamA) {
		// if it is, check if it has the flag
		if (Interactable->GetHasFlag()) {
			Interactable->ScoreGoal();
			// Spawn Another Flag using any random location from the array
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			GetWorld()->SpawnActor<ACpp_Flag>(FlagClass, FlagSpawnLocations[FMath::RandRange(0, FlagSpawnLocationsLength - 1)], FRotator::ZeroRotator, SpawnParams);			
		}		
	}
	
}

