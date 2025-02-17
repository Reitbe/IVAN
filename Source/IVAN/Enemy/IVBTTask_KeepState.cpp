// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBTTask_KeepState.h"
#include "BehaviorTree/BlackboardComponent.h"

UIVBTTask_KeepState::UIVBTTask_KeepState()
{
	NodeName = TEXT("KeepState");
	bNotifyTick = true;
	PreviousState = EMonsterState::Idle;
}

EBTNodeResult::Type UIVBTTask_KeepState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 블랙보드에서 현재 몬스터 상태 가져오기
	Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		PreviousState = static_cast<EMonsterState>(Blackboard->GetValueAsEnum(BBKEY_MONSTER_STATE));
	}

	// 현재 상태가 변경되기 전까지는 태스크를 계속 실행
	return EBTNodeResult::InProgress;
}

void UIVBTTask_KeepState::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// 현재 몬스터 상태가 태스크에 저장된 상태와 다르다면 태스크 종료
	if (Blackboard && PreviousState != static_cast<EMonsterState>(Blackboard->GetValueAsEnum(BBKEY_MONSTER_STATE)))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
