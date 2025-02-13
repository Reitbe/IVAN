// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBTTask_SpecialAttack.h"
#include "AIController.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "IVAN/Enemy/IVBlackBoardKeys.h"
#include "IVAN/Interface/IIVAICharacterSpecialCombat.h"

UIVBTTask_SpecialAttack::UIVBTTask_SpecialAttack()
{
}

EBTNodeResult::Type UIVBTTask_SpecialAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 블랙보드의 몬스터 상태 변경
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		//UE_LOG(LogTemp, Warning, TEXT("스페셜 무브 시작"));
		Blackboard->SetValueAsEnum(BBKEY_MONSTER_STATE, static_cast<uint8>(EMonsterState::SpecialAttack));

		// AI 캐릭터에게 스페셜 어택 명령을 전달
		IIIVAICharacterSpecialCombat* AICharacter = Cast<IIIVAICharacterSpecialCombat>(OwnerComp.GetAIOwner()->GetPawn());
		if (AICharacter)
		{
			AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(BBKEY_TARGET_ACTOR));
			AICharacter->SpecialAttack(TargetActor);
		}
	}
	return EBTNodeResult::Type();
}
