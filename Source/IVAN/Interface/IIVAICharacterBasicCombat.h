// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IIVAICharacterBasicCombat.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVAICharacterBasicCombat : public UInterface
{
	GENERATED_BODY()
};

/**
 * AI 캐릭터에 부착되어 공격 명령을 전달받는 인터페이스
 */
class IVAN_API IIIVAICharacterBasicCombat
{
	GENERATED_BODY()

public:
	virtual void ExecuteBasicAttack() = 0;
};
