// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Cpp_WGT_StartMatchTimer.h"
#include "Components/TextBlock.h"
#include "Core/Cpp_GS_CTF.h"


void UCpp_WGT_StartMatchTimer::UpdateMatchTimer(int newTime) {
	if (TXT_Timer) {
		TXT_Timer->SetText(FText::FromString(FString::FromInt(newTime) + "s"));
	}
	// Get GameState Reference
	SetGameStateReference(Cast<ACpp_GS_CTF>(GetWorld()->GetGameState()));

}

void UCpp_WGT_StartMatchTimer::NativeConstruct() {
	Super::NativeConstruct();
	// Set base text
	if (TXT_Timer) {
		TXT_Timer->SetText(FText::FromString("WAITING FOR PLAYERS!"));
	}
}

void UCpp_WGT_StartMatchTimer::SetGameStateReference(ACpp_GS_CTF* newGameState) {
	GS_CTF = newGameState;

	// Bind to the delegate
	GS_CTF->FMatchTimerUpdate.AddDynamic(this, &UCpp_WGT_StartMatchTimer::UpdateMatchTimer);
}
