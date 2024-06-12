// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Cpp_Projectile.h"

ACpp_Projectile::ACpp_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void ACpp_Projectile::BeginPlay()
{
	Super::BeginPlay();

	
}
void ACpp_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

