// Copyright Epic Games, Inc. All Rights Reserved.

#include "IVANGameMode.h"
#include "IVAN/Character/IVANCharacter.h"
#include "IVAN/GameSystem/IVDeathEventSubsystem.h"
#include "IVAN/Character/IVPlayerCharacter.h"
#include "IVAN/Controller/IVPlayerController.h"
#include "IVAN/UI/IVSimpleStatHUD.h"
#include "UObject/ConstructorHelpers.h"

AIVANGameMode::AIVANGameMode()
{
	// 모션 매칭 에러로 인해 주석 처리
	/*
	static ConstructorHelpers::FClassFinder<AIVPlayerController> PlayerControllerClassFinder 
	(TEXT("/Game/GameCore/PlayerControllers/BP_PlayerController.BP_PlayerController_C"));
	if (PlayerControllerClassFinder.Class)
	{
		PlayerControllerClass = PlayerControllerClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<AIVPlayerCharacter> DefaultPawnClassFinder
	(TEXT("/Game/Characters/IVCharacters/BP_PlayerCharacter.BP_PlayerCharacter_C"));
	if (DefaultPawnClassFinder.Class)
	{
		DefaultPawnClass = DefaultPawnClassFinder.Class;
	}
	*/

	static ConstructorHelpers::FClassFinder<AIVSimpleStatHUD> DefaultHUDClassFinder
	(TEXT("/Game/Widget/HUD_SimpleStat.HUD_SimpleStat_C"));
	if (DefaultHUDClassFinder.Class)
	{
		HUDClass = DefaultHUDClassFinder.Class;
	}
}

void AIVANGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 사망 이벤트 수신
	UIVDeathEventSubsystem* DeathEventSubsystem = GetGameInstance()->GetSubsystem<UIVDeathEventSubsystem>();
	if (DeathEventSubsystem)
	{
		DeathEventSubsystem->PlayerDeathEventDelegate.AddUObject(this, &AIVANGameMode::OnPlayerDeath);
	}
}

void AIVANGameMode::OnPlayerDeath()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("게임모드 - 플레이어 사망 수신."));
}
