// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Cpp_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"


ACpp_Projectile::ACpp_Projectile() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set actor to be Replicated
	bReplicates = true;
	SetReplicateMovement(true);

	// Create the UStaticMeshComponent* for ProjectileMesh
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;	
	// Set collision of mesh to NoCollision
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create a UProjectileMovementComponent for ProjectileMovement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	// Set projectile initial speed and MaxSpeed
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;


	// Set CollisionComponent Details
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));	
	CollisionComponent->SetupAttachment(RootComponent);
	// Set Size of CollisionComponent
	CollisionComponent->SetBoxExtent(FVector(11.f, 11.f, 11.f));
	// Set Collision Response to Ignore for all channels but For Pawn set it to Overlap
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// If projectile overlaps with another actor, call OnHit
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACpp_Projectile::OnHit);

	// After 5 seconds, destroy the projectile
	SetLifeSpan(5.f);
}


void ACpp_Projectile::BeginPlay() {
	Super::BeginPlay();

}
void ACpp_Projectile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);


}

void ACpp_Projectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit) {
	// only if has authority
	if (HasAuthority()) {
		// print Hit Actor name and destroy the projectile
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *OtherActor->GetName());
		Destroy();
		return;
	}

}

