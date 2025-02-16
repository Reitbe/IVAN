// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IIVAICharacterSpecialCombat.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVAICharacterSpecialCombat : public UInterface
{
	GENERATED_BODY()
};

/**
 * 보스 캐릭터에 부착되어 특수 공격 명령을 전달받는 인터페이스
 */
class IVAN_API IIIVAICharacterSpecialCombat
{
	GENERATED_BODY()

public:
	/* 태스크로부터 전달받아 특수 행동 진행*/
	virtual void SpecialMove(AActor* TargetActor) = 0;
	virtual void SpecialAttack(AActor* TargetActor) = 0;

	/* 노티파이로부터 전달받아 특수 행동 중 공격 진행 */
	virtual void OnNotifySpecialMoveHit() = 0;
	virtual void OnNotifySpecialAttackHit() = 0;
};
