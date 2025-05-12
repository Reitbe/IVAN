// Fill out your copyright notice in the Description page of Project Settings.


#include "IVANGameState.h"
#include "IVAN/GameSystem/IVSaveManagerSubsystem.h"

AIVANGameState::AIVANGameState()
{
	CurrentStageState = EStageState::FirstStage;
}

void AIVANGameState::BeginPlay()
{
	Super::BeginPlay();

	// 세이브 매니저
	UIVSaveManagerSubsystem* SaveManager = GetGameInstance()->GetSubsystem<UIVSaveManagerSubsystem>();
	if (SaveManager)
	{
		CurrentStageState = SaveManager->GetStageState(); // 저장된 스테이지 정보 가져오기
		SaveManager->OnSaveRequested.AddUObject(this, &AIVANGameState::SaveStageState); // 세이브 요청 시 스테이지 정보 저장
	}
}

void AIVANGameState::SaveStageState(UIVSaveGame* SaveGame)
{
	if (SaveGame)
	{
		SaveGame->SavedStageState = CurrentStageState;
	}
}
