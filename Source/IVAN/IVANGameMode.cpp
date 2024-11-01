// Copyright Epic Games, Inc. All Rights Reserved.

#include "IVANGameMode.h"
#include "IVANCharacter.h"
#include "UObject/ConstructorHelpers.h"

AIVANGameMode::AIVANGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
