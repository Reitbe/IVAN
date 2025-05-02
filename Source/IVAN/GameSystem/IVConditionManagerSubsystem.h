// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IVAN/IVGenericStructs.h"
#include "IVConditionManagerSubsystem.generated.h"

class AIVANGameState;
class UIVQuestManagerSubsystem;
class UIVInventoryComponent;
class UIVCharacterStatComponent;

/**
 * 
 */
UCLASS()
class IVAN_API UIVConditionManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

//기본
public:
	UIVConditionManagerSubsystem();
	
	/* 서브시스템 초기화 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;	


// 조건 확인 
public:
	/* 대화 시작 조건 확인 */
	FName CheckDialogueConditions(TArray<FDialogueEntry>& Conditions);

private:
	/* 종합 조건 검사의 시작점 */
	bool CheckCondition(const FConditionInfo& ConditionInfo);

	/* 스테이지 검사 */
	bool CheckStageCondition(EStageState StageStage, EConditionOperator Op, EStageState TargetStage);

	/* 퀘스트 검사 */
	bool CheckQuestCondition(EQuestState QuestState, EConditionOperator Op, EQuestState TargetState);

	/* 아이템 검사 */
	bool CheckItemCondition(FItemBaseInfo ItemInfo, EConditionOperator Op, float NumericValue);

	/* 스탯 검사 */
	bool CheckStatCondition(float StatValue, EConditionOperator Op, float NumericValue);


// 조건 가져오기
private:
	/* 스테이지 진행 상태 가져오기 */
	EStageState GetStageState();

	/* 퀘스트 진행 상태 가져오기 */
	EQuestState GetQuestState(FName QuestID);

	/* 아이템 정보 가져오기 */
	FItemBaseInfo GetItemInfo(FName ItemID);

	/* 스탯 정보 가져오기 */
	float GetStatInfo(EStatState StatType);


// 조건 제공 클래스
public:
	void SetGameState(AIVANGameState* NewGameState) { IVGameState = NewGameState; }
	void SetInventoryComponent(UIVInventoryComponent* NewInventoryComponent) { InventoryComponent = NewInventoryComponent; }
	void SetCharacterStatComponent(UIVCharacterStatComponent* NewCharacterStatComponent) { CharacterStatComponent = NewCharacterStatComponent; }

private:
	/* 스테이지 진행 정보를 가져오기 위한 게임 스테이트 */
	TObjectPtr<AIVANGameState> IVGameState;

	/* 퀘스트 진행 상태를 가져오기 위한 퀘스트 매니저 */
	TObjectPtr<UIVQuestManagerSubsystem> QuestManager;

	/* 아이템 정보를 가져올 인벤토리 컴포넌트 */
	TObjectPtr<UIVInventoryComponent> InventoryComponent;

	/* 스탯 정보를 가져올 스탯 컴포넌트 */
	TObjectPtr<UIVCharacterStatComponent> CharacterStatComponent;
};
