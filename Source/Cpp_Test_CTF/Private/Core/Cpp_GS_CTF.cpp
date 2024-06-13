// Fill out your copyright notice in the Description page of Project Settings.

// Game Includes
#include "Core/Cpp_GS_CTF.h"
#include "Actors/Cpp_RespawnPoints.h"
#include "../Cpp_Test_CTFCharacter.h"

// Engine Includes
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Widgets/Cpp_WGT_HUD.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"


void ACpp_GS_CTF::BeginPlay() {
	Super::BeginPlay();	

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
	int halfPlayers = PlayerCount / 2;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++) {
		APlayerController* PlayerController = It->Get();
		if (PlayerController) {
			ACpp_Test_CTFCharacter* Character = Cast<ACpp_Test_CTFCharacter>(PlayerController->GetCharacter());
			if (Character) {				
				Character->SetTeamA(halfPlayers > 0);
				if (halfPlayers > 0) {
					Character->SetRespawnPoints(RespawnPointsA);
				}
				else {
					Character->SetRespawnPoints(RespawnPointsB);
				}
				halfPlayers--;
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

void ACpp_GS_CTF::PlayerLoggedIn() {
	if (HasAuthority()) {
		PlayerCount++;
		if (PlayerCount >= MinPlayers) {
			PlayerCount++; // includes server
			MinPlayers = INT_MAX; // set to max value
			StartMatchTimer();
			
			// Get All Respawn Points from world
			TArray<AActor*> FoundPoints;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACpp_RespawnPoints::StaticClass(), FoundPoints);
			// Iterate through all found points and add to respective respawn points
			for (AActor* Actor : FoundPoints) {
				ACpp_RespawnPoints* RespawnPoint = Cast<ACpp_RespawnPoints>(Actor);
				if (RespawnPoint->GetIsTeamA() == 1) {
					RespawnPointsA.Add(RespawnPoint);
				}
				else {
					RespawnPointsB.Add(RespawnPoint);
				}
			}
			
		}
	}
}

void ACpp_GS_CTF::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACpp_GS_CTF, MatchTimer);
	DOREPLIFETIME(ACpp_GS_CTF, PlayerCount);
	DOREPLIFETIME(ACpp_GS_CTF, RespawnPointsA);
	DOREPLIFETIME(ACpp_GS_CTF, RespawnPointsB);
}

