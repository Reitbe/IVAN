// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "IVBTService_CalcDistance.generated.h"

/**
 * 지정 틱마다 타깃과의 거리를 계산하여 블랙보드에 저장하는 서비스
 */
UCLASS()
class IVAN_API UIVBTService_CalcDistance : public UBTService
{
	GENERATED_BODY()

public:
	UIVBTService_CalcDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
