// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/Cpp_InteractionInterface.h"

// Add default functionality here for any ICpp_InteractionInterface functions that are not pure virtual.

void ICpp_InteractionInterface::OnProjectileHit(AActor* OtherActor) {

}

bool ICpp_InteractionInterface::GetIsTeamA() {
    return false;
}

bool ICpp_InteractionInterface::GetHasFlag() {
	return false;
}

void ICpp_InteractionInterface::ScoreGoal() {

}