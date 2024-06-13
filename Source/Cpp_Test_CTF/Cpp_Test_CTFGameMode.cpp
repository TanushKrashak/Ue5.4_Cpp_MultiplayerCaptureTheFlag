// Copyright Epic Games, Inc. All Rights Reserved.

#include "Cpp_Test_CTFGameMode.h"
#include "Cpp_Test_CTFCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Core/Cpp_GS_CTF.h"

ACpp_Test_CTFGameMode::ACpp_Test_CTFGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ACpp_Test_CTFGameMode::BeginPlay() {
	Super::BeginPlay();
	
	// Get GameState
	GS_CTF = Cast<ACpp_GS_CTF>(GameState);
			
}


void ACpp_Test_CTFGameMode::OnPostLogin(AController* NewPlayer) {
	Super::OnPostLogin(NewPlayer);	
	if (GS_CTF) {
		GS_CTF->PlayerLoggedIn();
	}
}

