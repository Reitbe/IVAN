// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Controller/IVAIController.h"
#include "IVAN/Interface/IIVAIControllerSpecialCombat.h"
#include "IVBossAIController.generated.h"

/**
 * 보스 몬스터의 AI를 제어하기 위한 클래스. 현재는 램페이지만 사용중이다.
 * 일반 몬스터와는 다르게 SpecialMove, SpecialAttack을 사용한다. 
 */
UCLASS()
class IVAN_API AIVBossAIController : public AIVAIController, public IIIVAIControllerSpecialCombat
{
	GENERATED_BODY()

// 기본
public:
	AIVBossAIController();

protected:
	virtual void BeginPlay() override;
	
	
// 인터페이스
public:
	/* IIIVAIControllerSpecialCombat 인터페이스 구현*/
	virtual void SpecialMoveEnd() override;
	virtual void SpecialAttackEnd() override;
	
};
