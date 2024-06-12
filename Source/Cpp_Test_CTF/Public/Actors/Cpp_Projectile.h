// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cpp_Projectile.generated.h"

// Forward Declarations
class UStaticMeshComponent;
class UProjectileMovementComponent;

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




	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
};
