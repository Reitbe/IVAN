// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IVBTDecorator_SetRandomIndex.generated.h"

/**
 * 행동트리에서 자식 노드 중 하나를 랜덤하게 선택하는 데코레이터.
 * 데코레이터는 자식노드의 갯수를 가져와서 그 범위 내의 랜덤 인덱스를 블랙보드에 저장한다.
 * 자식 노드는 값 비교 데코레이터를 사용하여 본인의 인덱스와 블랙보드 값을 비교 후 실행한다.
 */
UCLASS()
class IVAN_API UIVBTDecorator_SetRandomIndex : public UBTDecorator
{
	GENERATED_BODY()

public:
	UIVBTDecorator_SetRandomIndex();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
