// Fill out your copyright notice in the Description page of Project Settings.

// Game Includes
#include "Core/Cpp_GS_CTF.h"
#include "Actors/Cpp_RespawnPoints.h"
#include "../Cpp_Test_CTFCharacter.h"
#include "Widgets/Cpp_WGT_GameEnd.h"

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
		BroadcastMatchTimer();		
	}
	else {
		if (HasAuthority()) {
			// Time For Match To End
			MatchTimer = 60;
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
				AllPlayers.Add(Character);
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
		if (HasAuthority()) {		
			EGameEndResult Result;
			if (TeamAScore > TeamBScore) {
				Result = EGameEndResult::GER_Win;
			}
			else if (TeamAScore < TeamBScore) {
				Result = EGameEndResult::GER_Lose;
			}
			else {
				int teamAKills = 0, teamBKills = 0;
				// Go Through All Players
				for (ACpp_Test_CTFCharacter* Character : AllPlayers) {
					if (Character) {
						if (Character->GetIsTeamA()) {
							teamAKills += Character->GetPlayerKills();
						}
						else {
							teamBKills += Character->GetPlayerKills();
						}
					}
				}
				if (teamAKills > teamBKills) {
					Result = EGameEndResult::GER_WinByKill;
				}
				else if (teamAKills < teamBKills) {
					Result = EGameEndResult::GER_LoseByKill;
				}
				else {
					Result = EGameEndResult::GER_Draw;
				}
			}
			// Create Game End Widget For All Players
			for (ACpp_Test_CTFCharacter* Character : AllPlayers) {
				if (Character) {
					Character->MC_CreateGameEndWidget(Result);
				}
			}
		}
			
	}
}

void ACpp_GS_CTF::BroadcastMatchTimer_Implementation() {
	FMatchTimerUpdate.Broadcast(MatchTimer);
}
void ACpp_GS_CTF::BroadcastGoalScoredA_Implementation() {
	FGoalScoredA.Broadcast(TeamAScore);
}
void ACpp_GS_CTF::BroadcastGoalScoredB_Implementation() {
	FGoalScoredB.Broadcast(TeamBScore);
}

void ACpp_GS_CTF::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACpp_GS_CTF, MatchTimer);
	DOREPLIFETIME(ACpp_GS_CTF, PlayerCount);
	DOREPLIFETIME(ACpp_GS_CTF, RespawnPointsA);
	DOREPLIFETIME(ACpp_GS_CTF, RespawnPointsB);
	DOREPLIFETIME(ACpp_GS_CTF, TeamAScore);
	DOREPLIFETIME(ACpp_GS_CTF, TeamBScore);
	DOREPLIFETIME(ACpp_GS_CTF, AllPlayers);
}

void ACpp_GS_CTF::OnRep_MatchTimer() {
	FMatchTimerUpdate.Broadcast(MatchTimer);
}
void ACpp_GS_CTF::OnRep_GoalScoredA() {
	FGoalScoredA.Broadcast(TeamAScore);
}
void ACpp_GS_CTF::OnRep_GoalScoredB() {
	FGoalScoredB.Broadcast(TeamBScore);	
}

void ACpp_GS_CTF::PlayerLoggedIn() {
	if (HasAuthority()) {
		PlayerCount++;
		if (PlayerCount >= MinPlayers) {
			PlayerCount++; // includes server
			MinPlayers = INT_MAX; // set to max value to prevent the event from firing again
			StartMatchTimer();
			// Call Start Match Timer Widget in 0.1 seconds
			/*FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACpp_GS_CTF::ShowMatchStartTimer, 0.1f, false);*/
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

void ACpp_GS_CTF::GoalScored(bool IsTeamA) {
	if (IsTeamA) {
		TeamAScore++;
		BroadcastGoalScoredA();
	}
	else {
		TeamBScore++;
		BroadcastGoalScoredB();
	}

}


//void ACpp_GS_CTF::ShowMatchStartTimer() {
//	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++) {
//		APlayerController* PlayerController = It->Get();
//		if (PlayerController) {
//			ACpp_Test_CTFCharacter* Character = Cast<ACpp_Test_CTFCharacter>(PlayerController->GetCharacter());
//			if (Character) {				
//				Character->MC_CreateMatchStartTimer(this, HUDWidgetClass);
//			}
//		}
//	}
//}

