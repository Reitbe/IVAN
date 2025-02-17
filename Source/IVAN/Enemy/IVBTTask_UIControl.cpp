// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBTTask_UIControl.h"
#include "AIController.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "IVAN/Enemy/IVBlackBoardKeys.h"
#include "IVAN/Interface/IIVAIUIControl.h"

UIVBTTask_UIControl::UIVBTTask_UIControl()
{
	NodeName = TEXT("UI Control");
}

EBTNodeResult::Type UIVBTTask_UIControl::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 거리 계산
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	IIIVAIUIControl* AIUIControl = Cast<IIIVAIUIControl>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (Blackboard && AIUIControl)
	{
		float Distance = Blackboard->GetValueAsFloat(BBKEY_DISTANCE_TO_TARGET);
		float Range = Blackboard->GetValueAsFloat(BBKEY_UI_ACTIVE_DISTANCE);
		bool bIsUIActivated = Blackboard->GetValueAsBool(BBKEY_HP_UI_STATE);
		
		// 캐릭터가 UI 활성화 범위에 있으며 현재 UI가 비활성화 상태라면 -> UI 활성화
		if (Distance < Range && !bIsUIActivated)
		{
			AIUIControl->ShowStatWidget();
			Blackboard->SetValueAsBool(BBKEY_HP_UI_STATE, true);
		}
		// 캐릭터가 UI 활성화 범위를 벗어났으며 현재 UI가 활성화 상태라면 -> UI 비활성화
		else if (Distance > Range && bIsUIActivated)
		{
			AIUIControl->HideStatWidget();
			Blackboard->SetValueAsBool(BBKEY_HP_UI_STATE, false);
		}
	}

	return EBTNodeResult::Type();
}
