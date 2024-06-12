// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Cpp_RespawnPoints.h"
#include "Components/ArrowComponent.h"

ACpp_RespawnPoints::ACpp_RespawnPoints() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create Arrow Component
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));

}


