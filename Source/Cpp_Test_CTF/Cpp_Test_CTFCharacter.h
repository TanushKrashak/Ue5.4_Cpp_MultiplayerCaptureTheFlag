// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Interfaces/Cpp_InteractionInterface.h"
#include "Cpp_Test_CTFCharacter.generated.h"

// Forward Declarations
class USpringArmComponent;
class UCameraComponent;
class UInputComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ACpp_RespawnPoints;
class UCpp_WGT_Respawning;
class ACpp_Flag;
class ACpp_GS_CTF;
class UCpp_WGT_HUD;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ACpp_Test_CTFCharacter : public ACharacter, public ICpp_InteractionInterface {
	GENERATED_BODY()
public:
	ACpp_Test_CTFCharacter();

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	// Projectile Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ACpp_Projectile> ProjectileClass;

	// Replicated Variables
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	bool bIsTeamA;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	TArray<ACpp_RespawnPoints*> RespawnPoints;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	bool bHasFlag;

	// Flag Reference
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	ACpp_Flag* Flag;

	// Respawn Widget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCpp_WGT_Respawning> RespawnWidgetClass;



	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	virtual void BeginPlay();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// Shoot a projectile
	UFUNCTION(Server, Reliable)
	void ShootProjectile();

	// Inherited via ICpp_InteractionInterface
	virtual bool GetIsTeamA() override;
	virtual void OnProjectileHit(AActor * OtherActor) override;
	virtual bool GetHasFlag() override;
	virtual void ScoreGoal() override;

	UFUNCTION(NetMulticast, Reliable)
	void OnDeath();
	void RespawnCharacter();

	// Required for replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	// Scene Component for Flag Attachment
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	USceneComponent* FlagAttachment;



	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(NetMulticast, Reliable)
	void SpawnCharacter();

	// Setters
	void SetFlag(ACpp_Flag* flag);

	UFUNCTION(NetMulticast, Reliable)
	void MC_CreateHUD(ACpp_GS_CTF* GameState, TSubclassOf<UCpp_WGT_HUD> HUDWidgetClass);
};

