// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBTDecorator_SetRandomIndex.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "Kismet/KismetMathLibrary.h"
#include "IVAN/Enemy/IVBlackBoardKeys.h"

UIVBTDecorator_SetRandomIndex::UIVBTDecorator_SetRandomIndex()
{
	NodeName = TEXT("Set Random Index");
}

bool UIVBTDecorator_SetRandomIndex::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// 테스크가 아닌 데코레이터기에 부모 노드를 먼저 가져온다
	UBTCompositeNode* IVParentNode = GetParentNode();
	if (!IVParentNode)
	{
		return false;
	}

	// 자식 노드의 수를 확인 후 랜덤 인덱스를 블랙보드에 저장
	int32 NumChildren = IVParentNode->GetChildrenNum();
	if (NumChildren > 0)
	{
		int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, NumChildren - 1);
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
		if (Blackboard)
		{
			Blackboard->SetValueAsInt(BBKEY_RANDOM_INDEX, RandomIndex);
		}
	}

	return true;
}

