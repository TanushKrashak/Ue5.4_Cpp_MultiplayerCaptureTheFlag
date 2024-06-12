// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Cpp_GS_CTF.generated.h"


UCLASS()
class CPP_TEST_CTF_API ACpp_GS_CTF : public AGameStateBase {
	GENERATED_BODY()

protected:
	uint8_t MatchTimer = 3;

	virtual void BeginPlay() override;

	void StartMatchTimer();
	void StartMatch();
};
