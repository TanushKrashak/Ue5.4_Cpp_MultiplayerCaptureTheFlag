// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Cpp_InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCpp_InteractionInterface : public UInterface {
	GENERATED_BODY()
};

class CPP_TEST_CTF_API ICpp_InteractionInterface {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Func for when projectile hits something
	UFUNCTION(Category = "Interaction")
	virtual void OnProjectileHit(AActor* OtherActor);

	// Func for when you need Player's Team
	UFUNCTION(Category = "Interaction")
	virtual bool GetIsTeamA();
};
