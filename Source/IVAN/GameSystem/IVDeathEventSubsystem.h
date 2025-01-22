// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IVDeathEventSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FMonsterDeathEventDelegate, AActor* /* 사망한 몬스터 */);
DECLARE_MULTICAST_DELEGATE(FPlayerDeathEventDelegate);
DECLARE_MULTICAST_DELEGATE(FPlayerRespawnEventDelegate);

/**
 * 플레이어 캐릭터 및 몬스터의 사망 이벤트를 관리하는 서브시스템
 */

UCLASS()
class IVAN_API UIVDeathEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/* 어떤 몬스터가 죽었음을 알려주는 대리자 */
	FMonsterDeathEventDelegate MonsterDeathEventDelegate;

	/* 플레이어가 죽었음을 알리는 대리자 */
	FPlayerDeathEventDelegate PlayerDeathEventDelegate;

	/* 플레이어가 부활했음을 알리는 대리자 */
	FPlayerRespawnEventDelegate PlayerRespawnEventDelegate;
};
