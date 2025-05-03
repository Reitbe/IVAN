// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IVAN/IVGenericStructs.h"
#include "IVDialogueDatabase.generated.h"

/**
 * 모든 대화 정보를 담고 있는 데이터베이스
 */
UCLASS()
class IVAN_API UIVDialogueDatabase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/* <대화 ID, 대화 세부 정보>를 담아둔 데이터베이스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TMap<FName, FDialogueInfo> DialogueEntries;
};
