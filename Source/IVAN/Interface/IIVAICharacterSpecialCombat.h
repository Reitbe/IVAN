// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IIVAICharacterSpecialCombat.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVAICharacterSpecialCombat : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IVAN_API IIIVAICharacterSpecialCombat
{
	GENERATED_BODY()

public:
	virtual void SpecialMove(AActor* TargetActor) = 0;
	virtual void SpecialAttack(AActor* TargetActor) = 0;
};
