// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Cpp_GS_CTF.generated.h"


UCLASS()
class CPP_TEST_CTF_API ACpp_GS_CTF : public AGameStateBase {
	GENERATED_BODY()
	
	// Beginplay like thing for Game State
	virtual void BeginPlay() override;
};
