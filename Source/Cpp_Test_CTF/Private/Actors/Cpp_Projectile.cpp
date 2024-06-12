// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Cpp_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACpp_Projectile::ACpp_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set actor to be Replicated
	bReplicates = true;
	SetReplicateMovement(true);

	// Create the UStaticMeshComponent* for ProjectileMesh
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;	

	// Create a UProjectileMovementComponent for ProjectileMovement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	// Set projectile initial speed and MaxSpeed
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;

}


void ACpp_Projectile::BeginPlay()
{
	Super::BeginPlay();

	
}
void ACpp_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

