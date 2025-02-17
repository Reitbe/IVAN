// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IVBTDecorator_CompareRange.generated.h"

/**
 * 두 거리를 입력받아 비교 결과를 출력하는 데코레이터
 * 주로 공격 범위와 타깃 사이의 거리를 비교할 때 사용한다.
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

	/* 지정 비교 범위 */
	UPROPERTY(EditAnywhere, Category = "DistanceParam")
	FBlackboardKeySelector DistanceRange;

};
