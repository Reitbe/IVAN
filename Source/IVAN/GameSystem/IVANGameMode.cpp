// Copyright Epic Games, Inc. All Rights Reserved.

#include "IVANGameMode.h"
#include "IVAN/Character/IVANCharacter.h"
#include "IVAN/GameSystem/IVDeathEventSubsystem.h"
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
}

void AIVANGameMode::RespawnPlayer(AIVPlayerCharacter* DeadCharacter)
{
	if (DeadCharacter)
	{
		// [임시] 월드의 플레이어 리스폰 포인트 가져오기
		TArray<AActor*> RespawnPoints;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), RespawnPointClass, RespawnPoints);

		// 위치 및 각도 설정
		FVector RespawnLocation = FVector::ZeroVector;
		if (RespawnPoints.Num() == 0)
		{
			RespawnLocation = FVector(900.0f, 1110.0f, 120.0f);
		}
		else
		{
			RespawnLocation = RespawnPoints[0]->GetActorLocation();
		}
		FRotator RespawnRotation = FRotator::ZeroRotator;

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
