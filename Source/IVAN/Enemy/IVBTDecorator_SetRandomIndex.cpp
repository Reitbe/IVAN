// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBTDecorator_SetRandomIndex.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "Kismet/KismetMathLibrary.h"
#include "IVAN/Enemy/IVBlackBoardKeys.h"

UIVBTDecorator_SetRandomIndex::UIVBTDecorator_SetRandomIndex()
{
	NodeName = TEXT("Set Random Index");
	RandomIndex = -1;
}

void UIVBTDecorator_SetRandomIndex::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	UBTCompositeNode* IVParentNode = GetParentNode();
	if (!IVParentNode) return;

	int32 NumChildren = IVParentNode->GetChildrenNum();
	if (NumChildren > 0)
	{
		RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, NumChildren - 1);
		UBlackboardComponent* Blackboard = SearchData.OwnerComp.GetBlackboardComponent();
		if (Blackboard)
		{
			Blackboard->SetValueAsInt(BBKEY_RANDOM_INDEX, RandomIndex);
		}

		UE_LOG(LogTemp, Warning, TEXT("NumChildren : %d, Random Index: %d"), NumChildren, RandomIndex);
	}
}

bool UIVBTDecorator_SetRandomIndex::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return false;
}

