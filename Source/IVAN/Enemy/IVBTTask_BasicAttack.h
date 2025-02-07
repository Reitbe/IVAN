// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "IVBTTask_BasicAttack.generated.h"

/**
 * 
 */
UCLASS()
class IVAN_API UIVBTTask_BasicAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UIVBTTask_BasicAttack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
