// Fill out your copyright notice in the Description page of Project Settings.


#include "IVDialogueManagerSubsystem.h"
#include "IVAN/GameSystem/IVConditionManagerSubsystem.h"
#include "IVAN/GameSystem/IVDatabaseSubsystem.h"

UIVDialogueManagerSubsystem::UIVDialogueManagerSubsystem()
{
}

void UIVDialogueManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 의존성이 있는 다른 서브시스템 먼저 초기화 지정
	Collection.InitializeDependency<UIVConditionManagerSubsystem>();
	Collection.InitializeDependency<UIVDatabaseSubsystem>();

	// 서브시스템 캐싱
	ConditionManagerSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UIVConditionManagerSubsystem>();
	DatabaseSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UIVDatabaseSubsystem>();
}

void UIVDialogueManagerSubsystem::StartDialogue(FName NPCID)
{
	// NPC ID에 해당하는 대화 시작 노드 찾기
	TArray<FDialogueEntry>& DialogueEntryDB = DatabaseSubsystem->GetDialogueEntryDatabase(NPCID);
	if (DialogueEntryDB.Num() == 0) return; // 참조라서 대화내역이 없는 DB가 있을 수 있다.

	// 대화 시작 조건 확인
	FName DialogueID = ConditionManagerSubsystem->CheckDialogueConditions(DialogueEntryDB);

	// 대화 진행
	ContinueDialogue(DialogueID);
}

void UIVDialogueManagerSubsystem::ContinueDialogue(const FName& DialogueID)
{
	FDialogueInfo& CurrentDialogue = DatabaseSubsystem->GetDialogueInfo(DialogueID);

	// 대화 모드 전달 
	OnDialogueModeSet.Broadcast(CurrentDialogue.DialogueID == NAME_None ? false : true);

	// 대화 내용 전달
	OnDialogueReady.Execute(CurrentDialogue);

}
