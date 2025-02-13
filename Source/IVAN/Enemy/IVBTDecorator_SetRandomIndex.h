// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IVBTDecorator_SetRandomIndex.generated.h"

/**
 * 행동트리에서 자식 노드 중 하나를 랜덤하게 선택하는 데코레이터
 */
UCLASS()
class IVAN_API UIVBTDecorator_SetRandomIndex : public UBTDecorator
{
	GENERATED_BODY()

public:
	UIVBTDecorator_SetRandomIndex();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;

private:
	UPROPERTY()
	int32 RandomIndex;
	
};
