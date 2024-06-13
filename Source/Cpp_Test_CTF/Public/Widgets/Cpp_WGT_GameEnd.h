// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Cpp_WGT_GameEnd.generated.h"

// Enum For Game End Result
UENUM()
enum class EGameEndResult : uint8 {
	GER_Win UMETA(DisplayName = "Win"),
	GER_Lose UMETA(DisplayName = "Lose"),
	GER_WinByKill UMETA(DisplayName = "WinByKill"),
	GER_LoseByKill UMETA(DisplayName = "LoseByKill"),
	GER_Draw UMETA(DisplayName = "Draw")
};

// Forward Declarations
class UTextBlock;
class UButton;

UCLASS()
class CPP_TEST_CTF_API UCpp_WGT_GameEnd : public UUserWidget {
	GENERATED_BODY()

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(BlueprintReadOnly, Category = "GameEnd", meta = (AllowPrivateAccess = "true"))
	EGameEndResult GameEndResult;

	// Text for Game End Result
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_GameEndResult;

	// Button For Closing Game
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* BTN_CloseGame;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CloseGame();

public:
	void SetGameEndResult(EGameEndResult Result);


};
