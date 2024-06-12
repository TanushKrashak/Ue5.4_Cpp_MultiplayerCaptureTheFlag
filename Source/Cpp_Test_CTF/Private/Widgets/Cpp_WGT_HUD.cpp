// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Cpp_WGT_HUD.h"
#include "Core/Cpp_GS_CTF.h"
#include "Components/TextBlock.h"


void UCpp_WGT_HUD::UpdateMatchTimer(int newTime) {
	if (TXT_Timer) {
		TXT_Timer->SetText(FText::FromString(FString::FromInt(newTime) + "s"));
	}
}

void UCpp_WGT_HUD::SetGameStateReference(ACpp_GS_CTF* newGameState) {
	GS_CTF = newGameState;

	// Bind to the delegate
	GS_CTF->FMatchTimerUpdate.AddDynamic(this, &UCpp_WGT_HUD::UpdateMatchTimer);
}



