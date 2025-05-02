// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 퀘스트, 대화 등 게인 진행에 종합적으로 사용되는 열거형 모음
 */

// 비교 조건
UENUM(BlueprintType)
enum class EConditionType : uint8
{
	None            UMETA(DisplayName = "None"),
	Stage           UMETA(DisplayName = "Stage"),
	Quest           UMETA(DisplayName = "Quest"),
	Item            UMETA(DisplayName = "Item"),
	Stat            UMETA(DisplayName = "Stat"),
};

// 비교 연산자
UENUM(BlueprintType)
enum class EConditionOperator : uint8
{
	None            UMETA(DisplayName = "None"),
	Equal           UMETA(DisplayName = "Equal"),
	NotEqual        UMETA(DisplayName = "NotEqual"),
	Greater         UMETA(DisplayName = "Greater"),
	Less            UMETA(DisplayName = "Less"),
	GreaterEqual    UMETA(DisplayName = "GreaterEqual"),
	LessEqual       UMETA(DisplayName = "LessEqual"),
	Has				UMETA(DisplayName = "Has"),
	NotHas			UMETA(DisplayName = "NotHas"),	
};

// 퀘스트 진행 상태
UENUM(BlueprintType)
enum class EQuestState : uint8
{
	None            UMETA(DisplayName = "None"),
	NotStarted      UMETA(DisplayName = "NotStarted"),
	InProgress      UMETA(DisplayName = "InProgress"),
	Completed       UMETA(DisplayName = "Completed"),
};

// 스테이지 상태
UENUM(BlueprintType)
enum class EStageState : uint8
{
	None            UMETA(DisplayName = "None"),
	FirstStage		UMETA(DisplayName = "FirstStage"),
	SecondStage		UMETA(DisplayName = "SecondStage"),
};

class IVAN_API IVGameProgressEnums
{
};
