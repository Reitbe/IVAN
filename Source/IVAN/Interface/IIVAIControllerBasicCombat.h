// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IIVAIControllerBasicCombat.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVAIControllerBasicCombat : public UInterface
{
	GENERATED_BODY()
};

/**
 * AI 컨트롤러에 부착되어 공격 명령을 전달받는 인터페이스
 */
class IVAN_API IIIVAIControllerBasicCombat
{
	GENERATED_BODY()

public:
	virtual void OnAttackEnd() = 0;
	virtual void OnHit(AActor* Attacker) = 0;
	virtual void OnHitEnd() = 0;
};
