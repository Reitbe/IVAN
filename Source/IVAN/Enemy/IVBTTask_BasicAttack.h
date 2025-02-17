// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "IVBTTask_BasicAttack.generated.h"

/**
 * AI의 기본 공격 행동을 수행하는 테스크
 * 실제 공격은 AI 캐릭터의 인터페이스를 통해 수행하며, 이곳에서는 블랙보드의 상태만을 변경한다.
 */
UCLASS()
class IVAN_API UIVBTTask_BasicAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UIVBTTask_BasicAttack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
