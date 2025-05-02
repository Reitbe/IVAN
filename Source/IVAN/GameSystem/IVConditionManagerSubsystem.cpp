// Fill out your copyright notice in the Description page of Project Settings.


#include "IVConditionManagerSubsystem.h"
#include "IVAN/Stat/IVCharacterStatComponent.h"
#include "IVAN/Item/IVInventoryComponent.h"
#include "IVAN/GameSystem/IVQuestManagerSubsystem.h"
#include "IVAN/GameSystem/IVANGameState.h"

UIVConditionManagerSubsystem::UIVConditionManagerSubsystem()
{
}

void UIVConditionManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 의존성이 있는 다른 서브시스템 먼저 초기화 지정
	Collection.InitializeDependency<UIVQuestManagerSubsystem>();

	// 서브시스템 캐싱
	QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UIVQuestManagerSubsystem>();

}

FName UIVConditionManagerSubsystem::CheckDialogueConditions(TArray<FDialogueEntry>& Conditions)
{
	FName Result = NAME_None;

	// 대화 엔트리 목록에서 각 엔트리별 대화 시작 조건을 확인
	for (auto& Entry : Conditions)
	{
		// 조건 확인
		bool bAllConditionsTrue = true;
		for (auto& Condition : Entry.Conditions)
		{
			if (!CheckCondition(Condition)) // 대화 조건 만족 시 출력할 대화 ID 반환
			{
				bAllConditionsTrue = false;
				break;
			}
		}

		if (bAllConditionsTrue)
		{
			return Entry.DialogueID; // 이번 대화의 모든 조건을 만족하는 경우 대화 ID 반환
		}
	}
	return Result; // 만족하는 조건이 없다면 NAME_None 반환
}

bool UIVConditionManagerSubsystem::CheckCondition(const FConditionInfo& ConditionInfo)
{
	// 조건 타입에 따라 분기
	switch (ConditionInfo.ConditionType) 
	{
	case EConditionType::None: 
		return false;

	case EConditionType::Stage:
		return CheckStageCondition(GetStageState(), ConditionInfo.ConditionOperator, ConditionInfo.StageStateValue);
	
	case EConditionType::Quest:
		return CheckQuestCondition(GetQuestState(ConditionInfo.PrimaryID), ConditionInfo.ConditionOperator, ConditionInfo.QuestStateValue);
	
	case EConditionType::Item:
		return CheckItemCondition(GetItemInfo(ConditionInfo.PrimaryID), ConditionInfo.ConditionOperator, ConditionInfo.NumericValue);
	
	case EConditionType::Stat:
		return CheckStatCondition(GetStatInfo(ConditionInfo.StatTypeValue), ConditionInfo.ConditionOperator, ConditionInfo.NumericValue);

	}

	return false;
}

bool UIVConditionManagerSubsystem::CheckStageCondition(EStageState StageStage, EConditionOperator Op, EStageState TargetStage)
{
	// 현재 스테이지와 타깃 스테이지 비교연산
	switch (Op)
	{
	case EConditionOperator::Equal:
		return StageStage == TargetStage;

	case EConditionOperator::NotEqual:
		return StageStage != TargetStage;

	case EConditionOperator::Greater:
		return StageStage > TargetStage;

	case EConditionOperator::Less:
		return StageStage < TargetStage;

	case EConditionOperator::GreaterEqual:
		return StageStage >= TargetStage;

	case EConditionOperator::LessEqual:
		return StageStage <= TargetStage;
	}

	return false;
}

bool UIVConditionManagerSubsystem::CheckQuestCondition(EQuestState QuestState, EConditionOperator Op, EQuestState TargetState)
{
	// 특정 퀘스트 진행 상태과 타깃 퀘스트 진행 상태 비교연산
	switch (Op)
	{
	case EConditionOperator::Equal:
		return QuestState == TargetState;

	case EConditionOperator::NotEqual:
		return QuestState != TargetState;

	case EConditionOperator::Greater:
		return QuestState > TargetState;

	case EConditionOperator::Less:
		return QuestState < TargetState;

	case EConditionOperator::GreaterEqual:
		return QuestState >= TargetState;

	case EConditionOperator::LessEqual:
		return QuestState <= TargetState;
	}

	return false;
}

bool UIVConditionManagerSubsystem::CheckItemCondition(FItemBaseInfo ItemInfo, EConditionOperator Op, float NumericValue)
{
	// 아이템 정보를 가져왔으면 존재하는지, 수량이 있는지 확인
	if (ItemInfo.ItemID == NAME_None) return false; // 아이템이 없다면 여기서 걸러진다.
	float ItemCount = ItemInfo.ItemCount;

	switch (Op)
	{
	case EConditionOperator::Equal:
		return ItemCount == NumericValue;

	case EConditionOperator::NotEqual:
		return ItemCount != NumericValue;

	case EConditionOperator::Greater:
		return ItemCount > NumericValue;

	case EConditionOperator::Less:
		return ItemCount < NumericValue;

	case EConditionOperator::GreaterEqual:
		return ItemCount >= NumericValue;

	case EConditionOperator::LessEqual:
		return ItemCount <= NumericValue;

	case EConditionOperator::Has: 
		return true;
	}

	return false;
}

bool UIVConditionManagerSubsystem::CheckStatCondition(float StatValue, EConditionOperator Op, float NumericValue)
{
	if (StatValue < 0.0f) return false; // 스탯이 없다면 여기서 걸러진다.

	switch (Op)
	{
	case EConditionOperator::Equal:
		return StatValue == NumericValue;

	case EConditionOperator::NotEqual:
		return StatValue != NumericValue;

	case EConditionOperator::Greater:
		return StatValue > NumericValue;

	case EConditionOperator::Less:
		return StatValue < NumericValue;

	case EConditionOperator::GreaterEqual:
		return StatValue >= NumericValue;

	case EConditionOperator::LessEqual:
		return StatValue <= NumericValue;
	}

	return false;
}

EStageState UIVConditionManagerSubsystem::GetStageState()
{
	if (IVGameState)
	{
		return IVGameState->GetStageState();
	}
	return EStageState::None;
}

EQuestState UIVConditionManagerSubsystem::GetQuestState(FName QuestID)
{
	// 퀘스트 매니저에서 퀘스트 정보를 가져온다. - 이후 추가
	return EQuestState();
}

FItemBaseInfo UIVConditionManagerSubsystem::GetItemInfo(FName ItemID)
{
	if (InventoryComponent)
	{
		return InventoryComponent->GetItemInfoFromInventoryByItemID(ItemID);
	}
	return FItemBaseInfo();
}

float UIVConditionManagerSubsystem::GetStatInfo(EStatState StatType)
{
	if (CharacterStatComponent)
	{
		return CharacterStatComponent->GetStatValue(StatType);
	}
	return -1.0f;
}


