// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IVBTDecorator_TaskEndTimer.generated.h"

/**
 * 지정된 만큼 일정 시간이 흐른 후 본 데코레이터를 장착한 태스크를 종료시키는 데코레이터
 * 현재는 작동하지 않는다.
 */
UCLASS()
class IVAN_API UIVBTDecorator_TaskEndTimer : public UBTDecorator
{
	GENERATED_BODY()

public:
	UIVBTDecorator_TaskEndTimer();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	FTimerHandle LimitTimerHandle;
};
