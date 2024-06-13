// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Cpp_WGT_GameEnd.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


void UCpp_WGT_GameEnd::NativeConstruct() {
	Super::NativeConstruct();

	// Bind Button Click Event
	if (BTN_CloseGame) {
		BTN_CloseGame->OnClicked.AddDynamic(this, &UCpp_WGT_GameEnd::CloseGame);
	}
	// Show Mouse Cursor & Set Input Mode UI Only
	GetOwningPlayer()->bShowMouseCursor = true;
	GetOwningPlayer()->SetInputMode(FInputModeUIOnly());

}

void UCpp_WGT_GameEnd::CloseGame() {	
	// Close Game For Server Directly
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}	

void UCpp_WGT_GameEnd::SetGameEndResult(EGameEndResult Result) {
	switch (Result) {
		// Set text For All Cases
		case EGameEndResult::GER_Win:
			TXT_GameEndResult->SetText(FText::FromString("Team A Wins!"));
			break;
		case EGameEndResult::GER_Lose:
			TXT_GameEndResult->SetText(FText::FromString("Team B Wins!"));
			break;
		case EGameEndResult::GER_WinByKill:
			TXT_GameEndResult->SetText(FText::FromString("Team A Wins By Kill!"));
			break;
		case EGameEndResult::GER_LoseByKill:
			TXT_GameEndResult->SetText(FText::FromString("Team B Wins By Kill!"));
			break;
		case EGameEndResult::GER_Draw:
			TXT_GameEndResult->SetText(FText::FromString("Match Draw!"));
			break;
	}
}