// Fill out your copyright notice in the Description page of Project Settings.


#include "IVDatabaseSubsystem.h"

UIVDatabaseSubsystem::UIVDatabaseSubsystem()
{
}

void UIVDatabaseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 아이템 데이터베이스 로드
	const FString ItemDatabasePath = TEXT("/Script/IVAN.IVItemDatabase'/Game/Items/DA_ItemDatabase.DA_ItemDatabase'");
	ItemDatabase = LoadObject<UIVItemDatabase>(nullptr, *ItemDatabasePath);

	// NPC별 대화 엔트리 데이터베이스 로드를 위한 구조체
	struct FEntry
	{
		FName KeyName; // NPC ID
		FString Path;  // 데이터베이스 경로
	};

	// 대화 엔트리 목록
	const TArray<FEntry> Entries =
	{
		{ FName(TEXT("OldMan")), TEXT("/Script/IVAN.IVDialogueEntryDatabase'/Game/GameCore/Database/DA_OldManDialogue.DA_OldManDialogue'") },
	};
	
	// 대화 엔트리 DB 로드
	for (auto& Entry : Entries)
	{
		if (UIVDialogueEntryDatabase* Database = LoadObject<UIVDialogueEntryDatabase>(nullptr, *Entry.Path))
		{
			DialogueEntryDatabase.Add(Entry.KeyName, *Database);
		}
	}

	// 전체 대화 DB 로드
	const FString DialogueDatabasePath = TEXT("/Script/IVAN.IVDialogueDatabase'/Game/GameCore/Database/DA_DialogueDatabase.DA_DialogueDatabase'");
	DialogueDatabase = LoadObject<UIVDialogueDatabase>(nullptr, *DialogueDatabasePath);
}

TArray<FDialogueEntry>& UIVDatabaseSubsystem::GetDialogueEntryDatabase(const FName& NPCID) const
{
	if (DialogueEntryDatabase.Contains(NPCID))
	{
		return DialogueEntryDatabase[NPCID]->DialogueEntries;
	}

	// 참조라서 대화 목록이 없더라도 반환하긴 해야함
	static TArray<FDialogueEntry> EmptyArray;
	return EmptyArray;
}

FDialogueInfo& UIVDatabaseSubsystem::GetDialogueInfo(const FName& DialogueID) const
{
	if (DialogueDatabase)
	{
		if (DialogueDatabase->DialogueEntries.Contains(DialogueID))
		{
			return DialogueDatabase->DialogueEntries[DialogueID];
		}
	}
	// 참조라서 대화 내용이 없더라도 반환하긴 해야함
	static FDialogueInfo EmptyInfo;
	return EmptyInfo;
}
