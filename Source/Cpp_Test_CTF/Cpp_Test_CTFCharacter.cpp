// Copyright Epic Games, Inc. All Rights Reserved.

// Game Includes
#include "Cpp_Test_CTFCharacter.h"
#include "Actors/Cpp_Projectile.h"
#include "Actors/Cpp_RespawnPoints.h"
#include "Widgets/Cpp_WGT_Respawning.h"
#include "Actors/Cpp_Flag.h"
#include "Core/Cpp_GS_CTF.h"
#include "Widgets/Cpp_WGT_HUD.h"
#include "Widgets/Cpp_WGT_StartMatchTimer.h"

// Engine Includes
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ACpp_Test_CTFCharacter::ACpp_Test_CTFCharacter() {
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	bIsDead = false;
	bHasFlag = false;
	PlayerKills = 0;

	// Set up Location For FlagAttachment
	FlagAttachment = CreateDefaultSubobject<USceneComponent>(TEXT("FlagAttachment"));
	FlagAttachment->SetupAttachment(RootComponent);
	FlagAttachment->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

}	

void ACpp_Test_CTFCharacter::BeginPlay() {
	// Call the base class  
	Super::BeginPlay();

	if (IsLocallyControlled()) {
		// Create Match Timer Widget
		StartMatchTimerWidget = CreateWidget<UCpp_WGT_StartMatchTimer>(GetWorld(), StartTimerWidgetClass);
		if (StartMatchTimerWidget) {			
			StartMatchTimerWidget->AddToViewport();
		}
	}

}
void ACpp_Test_CTFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACpp_Test_CTFCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACpp_Test_CTFCharacter::Look);

		// Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ACpp_Test_CTFCharacter::ShootProjectile);
	}
	else {
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACpp_Test_CTFCharacter::Move(const FInputActionValue& Value) {
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}
void ACpp_Test_CTFCharacter::Look(const FInputActionValue& Value) {
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACpp_Test_CTFCharacter::ShootProjectile_Implementation() {
	// Spawns a projectile class at the player's location (with offset) with the player's rotation
	if (ProjectileClass && !bIsDead && HasAuthority()) {		
		const FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f) + (GetActorForwardVector() * 100);
		const FRotator SpawnRotation = GetActorRotation();
		FActorSpawnParameters SpawnParams = FActorSpawnParameters();
		SpawnParams.Owner = this;
		// Spawn the projectile with transform and owner as this character
		GetWorld()->SpawnActor<ACpp_Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
	

}

void ACpp_Test_CTFCharacter::OnProjectileHit(AActor* OtherActor) {
	OnDeath();
}
void ACpp_Test_CTFCharacter::ScoreGoal() {
	bHasFlag = false;	
	// Destroy Flag
	Flag->Destroy();

}

void ACpp_Test_CTFCharacter::OnDeath_Implementation() {
	if (HasAuthority()) {
		// Character is dead
		bIsDead = true;
	}
	// hide and disable collision 
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	// Disable character movement
	GetCharacterMovement()->DisableMovement();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACpp_Test_CTFCharacter::RespawnCharacter, 2.0f, false);
	if (bHasFlag && HasAuthority()) {
		// Drop Flag			
		Flag->MC_SetFlagState(EFlagState::FS_Idle);		
		bHasFlag = false;			
		Flag = nullptr;
	}
	
	// Create Respawning Widget
	if (RespawnWidgetClass && IsLocallyControlled()) {
		UCpp_WGT_Respawning* RespawnWidget = CreateWidget<UCpp_WGT_Respawning>(GetWorld(), RespawnWidgetClass);
		if (RespawnWidget) {
			RespawnWidget->AddToViewport();
		}
	}
}
void ACpp_Test_CTFCharacter::RespawnCharacter() {
	bIsDead = false;
	// Enable character movement
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	// Randomly select a respawn point
	if (RespawnPoints.Num() > 0) {
		ACpp_RespawnPoints* RespawnPoint = RespawnPoints[FMath::RandRange(0, RespawnPoints.Num() - 1)];
		SetActorLocation(RespawnPoint->GetActorLocation());
		SetActorRotation(RespawnPoint->GetActorRotation());
	}
		// re-enable collision and unhide
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void ACpp_Test_CTFCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACpp_Test_CTFCharacter, bIsDead);
	DOREPLIFETIME(ACpp_Test_CTFCharacter, bIsTeamA);
	DOREPLIFETIME(ACpp_Test_CTFCharacter, RespawnPoints);	
	DOREPLIFETIME(ACpp_Test_CTFCharacter, bHasFlag);
	DOREPLIFETIME(ACpp_Test_CTFCharacter, Flag);
	DOREPLIFETIME(ACpp_Test_CTFCharacter, PlayerKills);
}

void ACpp_Test_CTFCharacter::MC_RemoveMatchStartWidget_Implementation() {
	if (IsLocallyControlled()) {
		if (StartMatchTimerWidget) {
			StartMatchTimerWidget->RemoveFromParent();
		}
	}
}

void ACpp_Test_CTFCharacter::SpawnCharacter_Implementation() {
	if (HasAuthority()) {
	// Randomly select a respawn point
		if (RespawnPoints.Num() > 0) {
			ACpp_RespawnPoints* RespawnPoint = RespawnPoints[FMath::RandRange(0, RespawnPoints.Num() - 1)];
			SetActorLocation(RespawnPoint->GetActorLocation());
			SetActorRotation(RespawnPoint->GetActorRotation());
			// Remove Start Match Timer Widget
			MC_RemoveMatchStartWidget();

		}
	}
}

bool ACpp_Test_CTFCharacter::GetIsDead() {
	return bIsDead;
}

int ACpp_Test_CTFCharacter::GetPlayerKills() {
	return PlayerKills;
}

void ACpp_Test_CTFCharacter::SetFlag(ACpp_Flag* flag) {
	bHasFlag = true;
	Flag = flag;
}

void ACpp_Test_CTFCharacter::SetTeamA(bool IsTeamA) {
	bIsTeamA = IsTeamA;
}

void ACpp_Test_CTFCharacter::SetRespawnPoints(const TArray<ACpp_RespawnPoints*>& InRespawnPoints) {
	RespawnPoints = InRespawnPoints;
}

void ACpp_Test_CTFCharacter::MC_CreateHUD_Implementation(ACpp_GS_CTF* GameState, TSubclassOf<UCpp_WGT_HUD> HUDWidgetClass) {
	// Create HUD widget
	if (HUDWidgetClass && IsLocallyControlled()) {		
		UCpp_WGT_HUD* HUDWidget = CreateWidget<UCpp_WGT_HUD>(GetWorld(), HUDWidgetClass);
		HUDWidget->SetGameStateReference(GameState);
		if (HUDWidget) {
			HUDWidget->AddToViewport();
		}
	}
}


void ACpp_Test_CTFCharacter::MC_UpdateKillCount_Implementation() {
	PlayerKills++;	
	FOnPlayerKillUpdate.Broadcast(PlayerKills);
}

bool ACpp_Test_CTFCharacter::GetIsTeamA() {
	return bIsTeamA;
}
bool ACpp_Test_CTFCharacter::GetHasFlag() {
	return bHasFlag;
}
