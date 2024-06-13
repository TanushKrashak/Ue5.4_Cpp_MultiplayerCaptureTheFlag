// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cpp_Flag.generated.h"

// Forward Declarations
class UStaticMeshComponent;
class UBoxComponent;

// Enum State
UENUM(BlueprintType)
enum class EFlagState : uint8 {
	FS_Idle UMETA(DisplayName = "Idle"),
	FS_Carried UMETA(DisplayName = "Carried"),	
};

UCLASS()
class CPP_TEST_CTF_API ACpp_Flag : public AActor {
	GENERATED_BODY()
	
public:	
	ACpp_Flag();

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	// Flag Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flag", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FlagMesh;

	// Flag Collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flag", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* FlagCollision;

	// Flag State
	EFlagState FlagState;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnFlagOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)
	void MC_OnFlagOverlap(AActor* OtherActor);
public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UFUNCTION(NetMulticast, Reliable)
	void MC_SetFlagState(EFlagState NewState);
};
