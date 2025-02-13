// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "IVBTTask_SpecialMove.generated.h"

/**
 * AI의 특수 이동 행동을 수행하는 테스크
 * 실제이동은 AI 캐릭터의 인터페이스를 통해 수행하며, 이곳에서는 블랙보드의 상태만을 변경한다.
 */
UCLASS()
class IVAN_API UIVBTTask_SpecialMove : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UIVBTTask_SpecialMove();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
