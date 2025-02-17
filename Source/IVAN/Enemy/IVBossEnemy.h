// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Enemy/IVEnemy.h"
#include "IVAN/Interface/IIVAICharacterSpecialCombat.h"
#include "IVBossEnemy.generated.h"

/**
 * 보스몬스터를 위한 베이스 클래스. 각 보스는 이를 상속받은 개별 클래스를 만들어 사용한다.
 * 사용 시 다중상속 문제를 조심할 것.
 */
UCLASS()
class IVAN_API AIVBossEnemy : public AIVEnemy, public IIIVAICharacterSpecialCombat
{
	GENERATED_BODY()

// 기본
public:
	AIVBossEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


// 인터페이스
public:
	/* IIIVAICharacterSpecialCombat 인터페이스 구현*/
	virtual void SpecialMove(AActor* TargetActor) override;
	virtual void SpecialAttack(AActor* TargetActor) override;
	virtual void OnNotifySpecialMoveHit() override;
	virtual void OnNotifySpecialAttackHit() override;

	
};
