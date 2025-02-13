// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IIVAIControllerSpecialCombat.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVAIControllerSpecialCombat : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IVAN_API IIIVAIControllerSpecialCombat
{
	GENERATED_BODY()

public:
	virtual void SpecialMoveEnd() = 0;
	virtual void SpecialAttackEnd() = 0;
};
