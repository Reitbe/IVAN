// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "IVAN/Enemy/IVBlackBoardKeys.h"
#include "IVBTTask_KeepState.generated.h"

class UBlackboardComponent;

/**
 * 앞선 태스크의 상태를 유지하기 위한 태스크.
 * 태스크 실행 시의 상태가 변경되기 전까지 계속 실행된다.
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
	TObjectPtr<UBlackboardComponent> Blackboard;

	/* 변경되기 이전의 노드 상태 */
	EMonsterState PreviousState;
};
