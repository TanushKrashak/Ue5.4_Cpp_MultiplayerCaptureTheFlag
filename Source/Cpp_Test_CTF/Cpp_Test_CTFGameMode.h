// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Cpp_Test_CTFGameMode.generated.h"

// Forward Declarations
class ACpp_GS_CTF;

UCLASS(minimalapi)
class ACpp_Test_CTFGameMode : public AGameModeBase {
	GENERATED_BODY()

public:
	ACpp_Test_CTFGameMode();

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================	
	// GameState Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameMode", meta = (AllowPrivateAccess = "true"))
	ACpp_GS_CTF* GS_CTF;



	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================	
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;



};



