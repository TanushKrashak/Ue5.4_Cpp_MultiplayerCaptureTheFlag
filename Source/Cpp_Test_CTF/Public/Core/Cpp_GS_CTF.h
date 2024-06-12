// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Cpp_GS_CTF.generated.h"


UCLASS()
class CPP_TEST_CTF_API ACpp_GS_CTF : public AGameStateBase {
	GENERATED_BODY()

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = "true"))
	int MatchTimer = 3; // 3 Seconds after which the match will start

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = "true"))
	int TeamAScore = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = "true"))
	int TeamBScore = 0;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	virtual void BeginPlay() override;

	void StartMatchTimer();
	void StartMatch();

	void HandleMatchTimer();
};
