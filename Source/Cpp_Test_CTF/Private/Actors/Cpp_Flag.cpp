// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Cpp_Flag.h"
#include "Components/BoxComponent.h"
#include "../Cpp_Test_CTFCharacter.h"


ACpp_Flag::ACpp_Flag() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set actor to be Replicated
	bReplicates = true;	
	SetReplicateMovement(true);

	// Create the Flag Mesh
	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flag Mesh"));
	FlagMesh->SetSimulatePhysics(true);
	FlagMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetRootComponent(FlagMesh);

	// Create the Flag Collision
	FlagCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Flag Collision"));	
	FlagCollision->SetupAttachment(FlagMesh);	
	FlagCollision->SetRelativeLocation(FVector(0.f, 0.f, 0.f));	
	FlagCollision->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	FlagCollision->OnComponentBeginOverlap.AddDynamic(this, &ACpp_Flag::OnFlagOverlap);	
	FlagCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);			

	FlagState = EFlagState::FS_Idle;			
}

void ACpp_Flag::BeginPlay() {
	Super::BeginPlay();
	
}

void ACpp_Flag::OnFlagOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {	
	if (HasAuthority()) {
		MC_OnFlagOverlap(OtherActor);
	}
}

void ACpp_Flag::MC_OnFlagOverlap_Implementation(AActor* OtherActor) {	
	switch (FlagState) {
		case EFlagState::FS_Idle:
			// Check if other actor was a player			
			if (auto* Player = Cast<ACpp_Test_CTFCharacter>(OtherActor)) {
				if (!Player->GetIsDead() && !Player->GetHasFlag()) {					
					FlagState = EFlagState::FS_Carried;
					Player->SetFlag(this);
					// Disable Self Collision and attach to player
					FlagMesh->SetSimulatePhysics(false);
					FlagMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					FlagCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					this->AttachToComponent(Player->FlagAttachment, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				}
			}
			break;
		case EFlagState::FS_Carried:
			break;
		default:
			break;
	}
}


void ACpp_Flag::MC_SetFlagState_Implementation(EFlagState NewState) {
	FlagState = NewState;
	if (FlagState == EFlagState::FS_Idle) {		
		// Detach from player character
		FlagMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);		
		// Reset the flag to its original position
		FlagMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		FlagCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		FlagMesh->SetSimulatePhysics(true);
	}
}
