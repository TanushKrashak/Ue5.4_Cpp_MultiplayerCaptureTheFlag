// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Cpp_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/Cpp_InteractionInterface.h"
#include "../Cpp_Test_CTFCharacter.h"



ACpp_Projectile::ACpp_Projectile() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set actor to be Replicated
	bReplicates = true;
	SetReplicateMovement(true);

	// Initialize ProjectileMesh Component 
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;	
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Initialize ProjectileMovement Component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;

	// Initialize Collision Component
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));	
	CollisionComponent->SetupAttachment(RootComponent);	
	CollisionComponent->SetBoxExtent(FVector(21.f, 21.f, 21.f));	
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACpp_Projectile::OnHit);

	// After 5 seconds, destroy the projectile
	SetLifeSpan(5.f);
}


void ACpp_Projectile::BeginPlay() {
	Super::BeginPlay();

}

void ACpp_Projectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit) {
	// only if has authority
	if (HasAuthority()) {
		// If the OtherActor is not null
		if (OtherActor) {
			// Call the OnProjectileHit function from the OtherActor
			ICpp_InteractionInterface* InteractionInterface = Cast<ICpp_InteractionInterface>(OtherActor);
			if (InteractionInterface) {
				// Call the OnProjectileHit function from the OtherActor
				InteractionInterface->OnProjectileHit(GetOwner());
				// Update kill count
				ACpp_Test_CTFCharacter* Character = Cast<ACpp_Test_CTFCharacter>(GetOwner());
				if (Character) {
					Character->MC_UpdateKillCount();
				}
			}
		}
		Destroy();
		return;
	}

}



