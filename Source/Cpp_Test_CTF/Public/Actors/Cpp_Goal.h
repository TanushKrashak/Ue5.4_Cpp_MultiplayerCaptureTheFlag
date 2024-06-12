// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cpp_Goal.generated.h"

// Forward Declarations
class UStaticMeshComponent;
class UBoxComponent;

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

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
};
