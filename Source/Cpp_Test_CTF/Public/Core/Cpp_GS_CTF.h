// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Cpp_GS_CTF.generated.h"

// Forward Declarations
class UCpp_WGT_HUD;

// Event Dispatcher for Match Timer
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchTimerUpdate, int, MatchTimer);


UCLASS()
class CPP_TEST_CTF_API ACpp_GS_CTF : public AGameStateBase {
	GENERATED_BODY()

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(ReplicatedUsing = OnRep_MatchTimer, VisibleAnywhere, BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = "true"))
	int MatchTimer = 3; // 3 Seconds after which the match will start

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = "true"))
	int TeamAScore = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = "true"))
	int TeamBScore = 0;

	// Hud Widget Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCpp_WGT_HUD> HUDWidgetClass;



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

public:
	// Delegate for Match Timer
	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnMatchTimerUpdate FMatchTimerUpdate;

	UFUNCTION()
	void OnRep_MatchTimer();

};