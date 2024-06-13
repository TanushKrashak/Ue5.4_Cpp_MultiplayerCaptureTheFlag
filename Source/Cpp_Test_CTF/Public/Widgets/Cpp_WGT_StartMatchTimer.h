// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Cpp_WGT_StartMatchTimer.generated.h"

// Forward Declarations
class UTextBlock;
class ACpp_GS_CTF;

UCLASS()
class CPP_TEST_CTF_API UCpp_WGT_StartMatchTimer : public UUserWidget {
	GENERATED_BODY()
	
protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================	
	// Text for the Timer
	UPROPERTY(BlueprintReadOnly, Category = "Timer", meta = (BindWidget))
	UTextBlock* TXT_Timer;

	// Reference to the Game State
	UPROPERTY()
	ACpp_GS_CTF* GS_CTF;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================	
	UFUNCTION()
	void UpdateMatchTimer(int newTime);
	
	virtual void NativeConstruct() override;


public:
	void SetGameStateReference(ACpp_GS_CTF* newGameState);

};
