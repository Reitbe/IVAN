// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IVBTDecorator_CompareRange.generated.h"

/**
 * 
 */
UCLASS()
class IVAN_API UIVBTDecorator_CompareRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UIVBTDecorator_CompareRange();

protected:
	/* 연산 결과를 바탕으로 데코레이터 동작 결정 */
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	/* 타깃 액터와 AI 사이의 거리 */
	UPROPERTY(EditAnywhere, Category = "DistanceParam")
	FBlackboardKeySelector DistanceToTarget;

	UPROPERTY(EditAnywhere, Category = "DistanceParam")
	FBlackboardKeySelector DistanceRange;

};
