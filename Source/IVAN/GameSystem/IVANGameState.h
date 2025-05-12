// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "IVAN/Enums/IVGameProgressEnums.h"
#include "IVANGameState.generated.h"

class UIVSaveGame;

/**
 * 게임 진행에 필요한 정보를 담아둔 클래스.
 */
UCLASS()
class IVAN_API AIVANGameState : public AGameStateBase
{
	GENERATED_BODY()

// 기본
public:
	AIVANGameState();
	virtual void BeginPlay() override;


// 스테이지 정보
public:
	EStageState GetStageState() const { return CurrentStageState; }
	void SetStageState(EStageState NewStageState){ CurrentStageState = NewStageState; }

private:
	/* 진행중인 스테이지 상태 */
	EStageState CurrentStageState;


// 세이브 시스템
public:
	/* 스테이지 정보를 세이브 매니저에 저장*/
	UFUNCTION()
	void SaveStageState(UIVSaveGame* SaveGame);
};
