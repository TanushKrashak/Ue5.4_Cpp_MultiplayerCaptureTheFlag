// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Cpp_GS_CTF.h"
#include "../Cpp_Test_CTFCharacter.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"


void ACpp_GS_CTF::BeginPlay() {
	Super::BeginPlay();
	
	// Call Start Match After 5 Seconds
	StartMatchTimer();

}

void ACpp_GS_CTF::StartMatchTimer() {
	if (MatchTimer > 0) {
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACpp_GS_CTF::StartMatchTimer, 1.0f, false);
		MatchTimer--;
	}
	else {
		StartMatch();
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
			}
		}
	}
}

