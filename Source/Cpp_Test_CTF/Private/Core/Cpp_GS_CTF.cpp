// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Cpp_GS_CTF.h"
#include "../Cpp_Test_CTFCharacter.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Widgets/Cpp_WGT_HUD.h"
#include "Net/UnrealNetwork.h"


void ACpp_GS_CTF::BeginPlay() {
	Super::BeginPlay();
		
	StartMatchTimer();

}

void ACpp_GS_CTF::StartMatchTimer() {
	if (MatchTimer > 0 && HasAuthority()) {
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACpp_GS_CTF::StartMatchTimer, 1.0f, false);
		MatchTimer--;
	}
	else {
		if (HasAuthority()) {
			// Time For Match To End
			MatchTimer = 5;
			StartMatch();
		}
	}
}

void ACpp_GS_CTF::StartMatch() {
	// Get All Players and call Respawn Character
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++) {
		APlayerController* PlayerController = It->Get();
		if (PlayerController) {
			ACpp_Test_CTFCharacter* Character = Cast<ACpp_Test_CTFCharacter>(PlayerController->GetCharacter());
			if (Character) {
				Character->SpawnCharacter();
				Character->MC_CreateHUD(this, HUDWidgetClass);							
			}
		}
	}
	

	// Start the timer for the match
	HandleMatchTimer();
}

void ACpp_GS_CTF::HandleMatchTimer() {
	if (MatchTimer > 0) {
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACpp_GS_CTF::HandleMatchTimer, 1.0f, false);
		MatchTimer--;
		BroadcastMatchTimer();
	}
	else {
		// End Match
		if (HasAuthority())
			UE_LOG(LogTemp, Warning, TEXT("Match Ended"));
	}
}

void ACpp_GS_CTF::BroadcastMatchTimer_Implementation() {
	FMatchTimerUpdate.Broadcast(MatchTimer);
}

void ACpp_GS_CTF::OnRep_MatchTimer() {
	FMatchTimerUpdate.Broadcast(MatchTimer);
}

void ACpp_GS_CTF::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACpp_GS_CTF, MatchTimer);
}

