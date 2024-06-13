// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Cpp_WGT_HUD.generated.h"

// Forward Declaration
class UTextBlock;
class ACpp_GS_CTF;

UCLASS()
class CPP_TEST_CTF_API UCpp_WGT_HUD : public UUserWidget {
	GENERATED_BODY()
	

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	// Timer 
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_Timer;
	// Scores
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_TeamAScore;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_TeamBScore;
	// Kill Count 
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_KillCount;

	// GameState Reference
	UPROPERTY()
	ACpp_GS_CTF* GS_CTF;


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	// Updation	
	UFUNCTION()
	void UpdateMatchTimer(int newTime);	
	UFUNCTION()
	void UpdateScoreA(int newScore);
	UFUNCTION()
	void UpdateScoreB(int newScore);
	UFUNCTION()
	void UpdateKillCount(int newKillCount);
	

public:
	void SetGameStateReference(ACpp_GS_CTF* newGameState);
};
