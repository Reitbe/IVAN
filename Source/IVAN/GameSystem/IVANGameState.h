// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "IVAN/Enums/IVGameProgressEnums.h"
#include "IVANGameState.generated.h"

/**
 * 
 */
UCLASS()
class IVAN_API AIVANGameState : public AGameStateBase
{
	GENERATED_BODY()

// 기본
public:
	AIVANGameState();


// 게임 상태
public:
	EStageState GetStageState() const { return CurrentStageState; }

private:
	/* 진행중인 스테이지 상태 */
	EStageState CurrentStageState;

	
};
