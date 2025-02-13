// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IVANGameMode.generated.h"

class AIVPlayerCharacter;

UCLASS(minimalapi)
class AIVANGameMode : public AGameModeBase
{
	GENERATED_BODY()

// 기본
public:
	AIVANGameMode();
	virtual void BeginPlay() override;


// 사망처리
public:
	/* 플레이어 스폰 액터 위치에서 플레이어 캐릭터를 재생성하여 부활 */
	void RespawnPlayer(AIVPlayerCharacter* DeadCharacter);

	void OnPlayerDeath();
	void OnPlayerAlive();

	/* 리스폰용 캐릭터 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameMode")
	TSubclassOf<AIVPlayerCharacter> RespawnCharacterClass;

	/* 리스폰 위치 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameMode")
	TSubclassOf<AActor> RespawnPointClass;
};



