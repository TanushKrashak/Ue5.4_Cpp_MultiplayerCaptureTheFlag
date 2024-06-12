// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cpp_Projectile.generated.h"

// Forward Declarations
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UBoxComponent;

UCLASS()
class CPP_TEST_CTF_API ACpp_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ACpp_Projectile();

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	// Add a UStaticMeshComponent* called ProjectileMesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;
	
	// Add a UProjectileMovementComponent* called ProjectileMovement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	// Collision Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionComponent;
		
	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Function to handle the projectile hitting something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
};
