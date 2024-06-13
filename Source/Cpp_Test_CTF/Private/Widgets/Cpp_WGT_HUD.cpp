// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Cpp_WGT_HUD.h"
#include "Core/Cpp_GS_CTF.h"
#include "Components/TextBlock.h"
#include "../Cpp_Test_CTFCharacter.h"


void UCpp_WGT_HUD::UpdateMatchTimer(int newTime) {
	if (TXT_Timer) {
		TXT_Timer->SetText(FText::FromString(FString::FromInt(newTime) + "s"));
	}
}

void UCpp_WGT_HUD::UpdateScoreA(int newScore) {
	if (TXT_TeamAScore) {
		TXT_TeamAScore->SetText(FText::FromString(FString::FromInt(newScore)));
	}
}

void UCpp_WGT_HUD::UpdateScoreB(int newScore) {
	if (TXT_TeamBScore) {
		TXT_TeamBScore->SetText(FText::FromString(FString::FromInt(newScore)));
	}
}

void UCpp_WGT_HUD::UpdateKillCount(int newKillCount) {
	if (TXT_KillCount) {
		TXT_KillCount->SetText(FText::FromString(FString::FromInt(newKillCount) + " Kills!"));
	}
}

void UCpp_WGT_HUD::SetGameStateReference(ACpp_GS_CTF* newGameState) {
	GS_CTF = newGameState;

	// Bind to the delegate
	GS_CTF->FMatchTimerUpdate.AddDynamic(this, &UCpp_WGT_HUD::UpdateMatchTimer);
	GS_CTF->FGoalScoredA.AddDynamic(this, &UCpp_WGT_HUD::UpdateScoreA);
	GS_CTF->FGoalScoredB.AddDynamic(this, &UCpp_WGT_HUD::UpdateScoreB);

	// Go Through All Players, If Valid and Locally Controlled, Bind to the Kill Count Update
	for (FConstPlayerControllerIterator It = GS_CTF->GetWorld()->GetPlayerControllerIterator(); It; It++) {
		APlayerController* PlayerController = It->Get();
		if (PlayerController) {
			ACpp_Test_CTFCharacter* Character = Cast<ACpp_Test_CTFCharacter>(PlayerController->GetCharacter());
			if (Character && Character->IsLocallyControlled()) {	
				UE_LOG(LogTemp, Warning, TEXT("Binding to Kill Count Update"));
				Character->FOnPlayerKillUpdate.AddDynamic(this, &UCpp_WGT_HUD::UpdateKillCount);
			}
		}
	}

}



