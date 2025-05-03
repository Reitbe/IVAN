// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IVAN/IVGenericStructs.h"
#include "IVDialogueManagerSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDialogueModeSet, bool /*DialogueMode On&Off*/);
DECLARE_DELEGATE_OneParam(FOnDialogueReady, const FDialogueInfo&);

class UIVConditionManagerSubsystem;
class UIVDatabaseSubsystem;

/**
 * 실제로 대화를 진행하는 서브시스템.
 * 1. DialogueComponent로부터 요청을 받고, 
 * 2. DB Subsystem에서 대화 시작 목록을 가져와, 
 * 3. Condition Subsystem에서 조건에 맞는 대화를 선택한 후, 
 * 4. DB Subsystem에서 출력할 대화 내용을 가져와서,
 * 5. HUD에 이를 전달한다. 
 * 6. 대화가 이어진다면 4~5를 반복한다. 
 */
UCLASS()
class IVAN_API UIVDialogueManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

// 기본
public:
	UIVDialogueManagerSubsystem();

	/* 서브시스템 초기화 - DB및 조건확인 서브시스템 의존 */ 
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;


// 다른 서브시스템 캐싱
private:
	/* 현재 상태 기반으로 어떤 대화를 시작할지 고르기 위한 조건확인 서브시스템*/
	TObjectPtr<UIVConditionManagerSubsystem> ConditionManagerSubsystem;

	/* 가능한 대화 목록과 대화 내용을 저장한 DB 서브시스템*/
	TObjectPtr<UIVDatabaseSubsystem> DatabaseSubsystem;


// 대화 관련
public:	
	/* 대화 시작 */
	void StartDialogue(FName NPCID);

	/* 대화 진행 */
	void ContinueDialogue(const FName& DialogueID);

	/* 대화 전달용 대리자 */
	FOnDialogueReady OnDialogueReady;

	/* 대화 모드 전달용 대리자 */
	FOnDialogueModeSet OnDialogueModeSet;
};
