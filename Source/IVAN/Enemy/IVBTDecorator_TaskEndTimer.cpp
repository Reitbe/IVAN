// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBTDecorator_TaskEndTimer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "Kismet/KismetMathLibrary.h"
#include "IVAN/Enemy/IVBlackBoardKeys.h"

UIVBTDecorator_TaskEndTimer::UIVBTDecorator_TaskEndTimer()
{
	NodeName = TEXT("Task End Timer");
}

bool UIVBTDecorator_TaskEndTimer::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	return false;
}
