// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IVAN/Item/IVItemDatabase.h"
#include "IVAN/Dialogue/IVDialogueEntryDatabase.h"
#include "IVAN/Dialogue/IVDialogueDatabase.h"
#include "IVDatabaseSubsystem.generated.h"

/**
 * 데이터베이스 로드 및 관리용 서브시스템
 */
UCLASS()
class IVAN_API UIVDatabaseSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UIVDatabaseSubsystem();

	/* 데이터베이스 서브시스템 초기화 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;


// 데이터베이스 접근용
public:
	/* 아이템 데이터베이스 접근용 */
	UFUNCTION(BlueprintCallable)
	UIVItemDatabase* GetItemDatabase() const { return ItemDatabase.Get(); }

	/* 대화 시작 데이터베이스 접근용 */
	UFUNCTION(BlueprintCallable)
	TArray<FDialogueEntry>& GetDialogueEntryDatabase(const FName& NPCID) const;

	/* 대화 내용 데이터베이스에서 특정 대화 접근용 */
	UFUNCTION(BlueprintCallable)
	FDialogueInfo& GetDialogueInfo(const FName& DialogueID) const;


// 실제 데이터베이스들
private:
	/* 아이템 데이터베이스 */
	TObjectPtr<UIVItemDatabase> ItemDatabase;

	/* 대화 시작 데이터베이스 - NPC ID별로 다른 데이터베이스 로드*/
	TMap<FName, TObjectPtr<UIVDialogueEntryDatabase>> DialogueEntryDatabase;

	/* 대화 내용 데이터베이스 */
	TObjectPtr<UIVDialogueDatabase> DialogueDatabase;
};
