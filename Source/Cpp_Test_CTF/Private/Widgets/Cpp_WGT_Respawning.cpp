// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Cpp_WGT_Respawning.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

void UCpp_WGT_Respawning::NativeConstruct() {
	Super::NativeConstruct();
	// Destroy the widget after 2 seconds
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCpp_WGT_Respawning::RemoveFromParent, 2.0f, false);

	// Set the text to the respawn message
	TXT_Respawn->SetText(FText::FromString("Respawning..."));
	
}
