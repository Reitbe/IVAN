// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBTTask_SpecialMove.h"
#include "AIController.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "IVAN/Enemy/IVBlackBoardKeys.h"
#include "IVAN/Interface/IIVAICharacterSpecialCombat.h"

UIVBTTask_SpecialMove::UIVBTTask_SpecialMove()
{
}

EBTNodeResult::Type UIVBTTask_SpecialMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 블랙보드의 몬스터 상태 변경
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		Blackboard->SetValueAsEnum(BBKEY_MONSTER_STATE, static_cast<uint8>(EMonsterState::SpecialMove));

		// AI 캐릭터에게 스페셜 무브 명령을 전달
		IIIVAICharacterSpecialCombat* AICharacter = Cast<IIIVAICharacterSpecialCombat>(OwnerComp.GetAIOwner()->GetPawn());
		if (AICharacter)
		{
			AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(BBKEY_TARGET_ACTOR));
			AICharacter->SpecialMove(TargetActor);
		}
	}
	return EBTNodeResult::Type();
}
