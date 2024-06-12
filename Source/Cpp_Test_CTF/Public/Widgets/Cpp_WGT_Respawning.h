// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Cpp_WGT_Respawning.generated.h"

// Forward Declarations
class UTextBlock;

UCLASS()
class CPP_TEST_CTF_API UCpp_WGT_Respawning : public UUserWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* TXT_Respawn;

	// Construct
	virtual void NativeConstruct() override;
};
