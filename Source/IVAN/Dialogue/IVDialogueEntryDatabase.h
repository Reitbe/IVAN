// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IVAN/IVGenericStructs.h"
#include "IVDialogueEntryDatabase.generated.h"

/**
 * NPC별로 접근 가능한 '대화 시작 지점' 정보를 담은 데이터베이스.
 * 이를 기반으로 NPC마다 별도의 DataAsset을 생성한다.
 */
UCLASS()
class IVAN_API UIVDialogueEntryDatabase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/* 데이터베이스에 저장된 대화 시작 노드의 세부 정보들 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TArray<FDialogueEntry> DialogueEntries;
};
