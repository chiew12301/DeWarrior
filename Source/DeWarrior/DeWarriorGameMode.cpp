// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeWarriorGameMode.h"
#include "DeWarriorCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADeWarriorGameMode::ADeWarriorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
