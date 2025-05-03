// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "IVAN/Enums/IVGameProgressEnums.h"
#include "IVANGameState.generated.h"

/**
 * 게임 진행에 필요한 정보를 담아둔 클래스.
 * 아직 스테이지 정보만 담고 있다. 
 */
UCLASS()
class IVAN_API AIVANGameState : public AGameStateBase
{
	GENERATED_BODY()

// 기본
public:
	AIVANGameState();


// 스테이지 정보
public:
	EStageState GetStageState() const { return CurrentStageState; }
	void SetStageState(EStageState NewStageState){ CurrentStageState = NewStageState; }

private:
	/* 진행중인 스테이지 상태 */
	EStageState CurrentStageState;

	
};
