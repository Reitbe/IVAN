// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "IVBTService_CalcDistance.generated.h"

/**
 * 
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
