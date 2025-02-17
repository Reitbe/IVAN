// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBTDecorator_CompareRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "IVAN/Enemy/IVBlackBoardKeys.h"

UIVBTDecorator_CompareRange::UIVBTDecorator_CompareRange()
{
	NodeName = TEXT("Compare (Distance < Range)");
}

bool UIVBTDecorator_CompareRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// 블랙보드 정보를 가져와서 값 비교
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	float Distance = BlackboardComp->GetValueAsFloat(DistanceToTarget.SelectedKeyName);
	float Range = BlackboardComp->GetValueAsFloat(DistanceRange.SelectedKeyName);

	return Distance < Range;
}
