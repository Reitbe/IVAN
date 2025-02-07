// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBTTask_BasicAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "IVAN/Enemy/IVBlackBoardKeys.h"
#include "IVAN/Interface/IIVAICharacterBasicCombat.h"

UIVBTTask_BasicAttack::UIVBTTask_BasicAttack()
{
}

EBTNodeResult::Type UIVBTTask_BasicAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// AI 캐릭터에게 공격 명령을 전달
	IIIVAICharacterBasicCombat* AICharacter = Cast<IIIVAICharacterBasicCombat>(OwnerComp.GetAIOwner()->GetPawn());
	if (AICharacter)
	{
		AICharacter->ExecuteBasicAttack();
	}

	// 블랙보드의 몬스터 상태 변경
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		Blackboard->SetValueAsEnum(BBKEY_MONSTER_STATE, static_cast<uint8>(EMonsterState::BasicAttack));
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("기본 공격 태스크"));

	return EBTNodeResult::Type();
}
