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
	// DefaultPawnClasss는 모션 매칭 에러로 인해 생략
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
	UIVDeathEventSubsystem* LifeEventSubsystem = GetGameInstance()->GetSubsystem<UIVDeathEventSubsystem>();
	if (LifeEventSubsystem)
	{
		LifeEventSubsystem->PlayerDeathEventDelegate.AddUObject(this, &AIVANGameMode::OnPlayerDeath);
		LifeEventSubsystem->PlayerRespawnEventDelegate.AddUObject(this, &AIVANGameMode::OnPlayerAlive);
	}
}

void AIVANGameMode::OnPlayerDeath()
{
}

void AIVANGameMode::OnPlayerAlive()
{
}
