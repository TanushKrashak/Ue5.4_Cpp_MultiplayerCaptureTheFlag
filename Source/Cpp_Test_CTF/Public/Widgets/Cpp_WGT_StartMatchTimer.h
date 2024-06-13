// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Cpp_WGT_StartMatchTimer.generated.h"

// Forward Declarations
class UTextBlock;


UCLASS()
class CPP_TEST_CTF_API UCpp_WGT_StartMatchTimer : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// Text for the Timer
	UPROPERTY(BlueprintReadOnly, Category = "Timer", meta = (BindWidget))
	UTextBlock* TimerText;

public:

};
