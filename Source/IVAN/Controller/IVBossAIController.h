// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Controller/IVAIController.h"
#include "IVAN/Interface/IIVAIControllerSpecialCombat.h"
#include "IVBossAIController.generated.h"

/**
 * 
 */
UCLASS()
class IVAN_API AIVBossAIController : public AIVAIController, public IIIVAIControllerSpecialCombat
{
	GENERATED_BODY()

// 기본
public:
	AIVBossAIController();

protected:
	virtual void BeginPlay() override;
	
	
// 인터페이스
public:
	/* IIIVAIControllerSpecialCombat 인터페이스 구현*/
	virtual void SpecialMoveEnd() override;
	virtual void SpecialAttackEnd() override;
	
};
