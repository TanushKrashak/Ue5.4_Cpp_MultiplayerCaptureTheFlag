// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Cpp_GS_CTF.generated.h"

// Forward Declarations
class UCpp_WGT_HUD;
class ACpp_RespawnPoints;
class ACpp_Test_CTFCharacter;

// Event Dispatcher for Match Timer
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchTimerUpdate, int, MatchTimer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoalScoredA, int, TeamAScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoalScoredB, int, TeamBScore);


UCLASS()
class CPP_TEST_CTF_API ACpp_GS_CTF : public AGameStateBase {
	GENERATED_BODY()

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(ReplicatedUsing = OnRep_MatchTimer, EditAnywhere, BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = "true"))
	int MatchTimer = 3; // 3 Seconds after which the match will start

	UPROPERTY(ReplicatedUsing = OnRep_GoalScoredA, VisibleAnywhere, BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = "true"))
	int TeamAScore = 0;
	UPROPERTY(ReplicatedUsing = OnRep_GoalScoredB, VisibleAnywhere, BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = "true"))
	int TeamBScore = 0;

	// Hud Widget Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCpp_WGT_HUD> HUDWidgetClass;

	// Player Count
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = "true"))
	int PlayerCount = 0;

	/// Minimum Players Required for the match to start (DOES NOT COUNT SERVER)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int MinPlayers = 1;
	
	// Respawn Points
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	TArray<ACpp_RespawnPoints*> RespawnPointsA;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	TArray<ACpp_RespawnPoints*> RespawnPointsB;

	// All Players
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	TArray<ACpp_Test_CTFCharacter*> AllPlayers;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	virtual void BeginPlay() override;

	void StartMatchTimer();
	void StartMatch();

	void HandleMatchTimer();

	// Broadcasts For Delegates
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastMatchTimer();
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastGoalScoredA();
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastGoalScoredB();

	// Required For Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//void ShowMatchStartTimer();
	

public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	// Delegates for Match Timer
	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnMatchTimerUpdate FMatchTimerUpdate;
	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnGoalScoredA FGoalScoredA;
	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnGoalScoredB FGoalScoredB;



	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	// Functions For Broadcasting to Widgets
	UFUNCTION()
	void OnRep_MatchTimer();
	UFUNCTION()
	void OnRep_GoalScoredA();
	UFUNCTION()
	void OnRep_GoalScoredB();

	// Player Login
	void PlayerLoggedIn();

	// On Goal Scored
	void GoalScored(bool IsTeamA);

};