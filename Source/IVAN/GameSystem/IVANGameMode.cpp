// Copyright Epic Games, Inc. All Rights Reserved.

#include "IVANGameMode.h"
#include "IVAN/Character/IVANCharacter.h"
#include "IVAN/GameSystem/IVDeathEventSubsystem.h"
#include "IVAN/GameSystem/IVConditionManagerSubsystem.h"
#include "IVAN/GameSystem/IVSaveManagerSubsystem.h"
#include "IVAN/Character/IVPlayerCharacter.h"
#include "IVAN/Controller/IVPlayerController.h"
#include "IVAN/UI/IVSimpleStatHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AIVANGameMode::AIVANGameMode()
{
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

	// 조건 확인 서브시스템에 GameState 설정 - 게임모드에 GameState 설정 필수
	UIVConditionManagerSubsystem* ConditionManager = GetGameInstance()->GetSubsystem<UIVConditionManagerSubsystem>();
	if (ConditionManager)
	{
		AIVANGameState* IVGameState = Cast<AIVANGameState>(UGameplayStatics::GetGameState(this));
		if (IVGameState)
		{
			ConditionManager->SetGameState(IVGameState);
		}
	}
}

void AIVANGameMode::RespawnPlayer(AIVPlayerCharacter* DeadCharacter)
{
	if (DeadCharacter)
	{
		// 세이브 매니저에 저장된 리스폰 지점을 사용
		UIVSaveManagerSubsystem* SaveManager = GetGameInstance()->GetSubsystem<UIVSaveManagerSubsystem>();
		FTransform SpawnTransform = (SaveManager) ? SaveManager->GetTransform() : FTransform();
		FVector RespawnLocation = SpawnTransform.GetLocation();
		FRotator RespawnRotation = SpawnTransform.GetRotation().Rotator();

		AController* PlayerController = DeadCharacter->GetController();

		// 기존 캐릭터 제거 및 새로운 캐릭터 스폰
		DeadCharacter->Destroy();
		AIVPlayerCharacter* NewCharacter = GetWorld()->SpawnActor<AIVPlayerCharacter>(RespawnCharacterClass, RespawnLocation, RespawnRotation);
		if (PlayerController && NewCharacter)
		{
			PlayerController->Possess(NewCharacter);
		}
	}
}

void AIVANGameMode::OnPlayerDeath()
{
}

void AIVANGameMode::OnPlayerAlive()
{

}
