// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cpp_Goal.generated.h"

// Forward Declarations
class UStaticMeshComponent;
class UBoxComponent;
class UMaterialInstance;
class ACpp_Flag;

UCLASS()
class CPP_TEST_CTF_API ACpp_Goal : public AActor {
	GENERATED_BODY()
	
public:	
	ACpp_Goal();

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	// Goal Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Goal", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* GoalMesh;

	// Goal Collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Goal", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* GoalCollision;

	// Team Value
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal", meta = (AllowPrivateAccess = "true"))
	bool bIsTeamA;

	// Material for Team A
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* TeamAMaterial;

	// Material for Team B
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* TeamBMaterial;

	// Possible Spawn Locations For Flags
	UPROPERTY(BlueprintReadOnly, Category = "Goal", meta = (AllowPrivateAccess = "true"))
	TArray<FVector> FlagSpawnLocations;

	// Length of Flag Spawn Locations
	uint8_t FlagSpawnLocationsLength;

	// Flag Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACpp_Flag> FlagClass;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnGoalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)
	void MC_OnGoalOverlap(AActor* OtherActor);

public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================


};
