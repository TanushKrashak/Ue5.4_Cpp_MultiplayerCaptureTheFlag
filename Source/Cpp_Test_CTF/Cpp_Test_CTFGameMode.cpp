// Copyright Epic Games, Inc. All Rights Reserved.

#include "Cpp_Test_CTFGameMode.h"
#include "Cpp_Test_CTFCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACpp_Test_CTFGameMode::ACpp_Test_CTFGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
