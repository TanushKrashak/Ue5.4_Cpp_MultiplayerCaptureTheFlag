// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Cpp_GS_CTF.generated.h"

// Forward Declarations
class UCpp_WGT_HUD;
class ACpp_RespawnPoints;

// Event Dispatcher for Match Timer
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchTimerUpdate, int, MatchTimer);


UCLASS()
class CPP_TEST_CTF_API ACpp_GS_CTF : public AGameStateBase {
	GENERATED_BODY()

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(ReplicatedUsing = OnRep_MatchTimer, EditAnywhere, BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = "true"))
	int MatchTimer = 3; // 3 Seconds after which the match will start

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = "true"))
	int TeamAScore = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = "true"))
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

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	virtual void BeginPlay() override;

	void StartMatchTimer();
	void StartMatch();

	void HandleMatchTimer();

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastMatchTimer();

	// Required For Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//void ShowMatchStartTimer();
	

public:
	// Delegate for Match Timer
	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnMatchTimerUpdate FMatchTimerUpdate;

	UFUNCTION()
	void OnRep_MatchTimer();

	// Player Login
	void PlayerLoggedIn();

};