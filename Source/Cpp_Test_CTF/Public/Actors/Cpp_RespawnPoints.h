// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cpp_RespawnPoints.generated.h"

// Forward Declarations
class UArrowComponent;

UCLASS()
class CPP_TEST_CTF_API ACpp_RespawnPoints : public AActor {
	GENERATED_BODY()
	
public:	
	ACpp_RespawnPoints();

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
	bool bIsTeamA;

	// Add Arrow Component to show direction
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Respawn")
	class UArrowComponent* ArrowComponent;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================

public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	bool GetIsTeamA();
};
