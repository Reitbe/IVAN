// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Enemy/IVEnemy.h"
#include "IVAN/Interface/IIVAICharacterSpecialCombat.h"
#include "IVBossEnemy.generated.h"

/**
 * 
 */
UCLASS()
class IVAN_API AIVBossEnemy : public AIVEnemy, public IIIVAICharacterSpecialCombat
{
	GENERATED_BODY()

// 기본
public:
	AIVBossEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


// 인터페이스
public:
	/* IIIVAICharacterSpecialCombat 인터페이스 구현*/
	virtual void SpecialMove(AActor* TargetActor) override;
	virtual void SpecialAttack(AActor* TargetActor) override;
	
};
