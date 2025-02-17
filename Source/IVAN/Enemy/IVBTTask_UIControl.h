// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "IVBTTask_UIControl.generated.h"

/**
 * 몬스터 상단의 체력 UI 제어용 태스크
 */
UCLASS()
class IVAN_API UIVBTTask_UIControl : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UIVBTTask_UIControl();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
