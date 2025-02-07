// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "IVAN/Enemy/IVBlackBoardKeys.h"
#include "IVBTTask_KeepState.generated.h"

class UBlackboardComponent;

/**
 * 
 */
UCLASS()
class IVAN_API UIVBTTask_KeepState : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UIVBTTask_KeepState();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	/* 블랙보드 컴포넌트 - 틱용 */
	TObjectPtr<UBlackboardComponent> Blackboard;

	/* 새로 변경되기 이전의 노드 상태 */
	EMonsterState PreviousState;
};
