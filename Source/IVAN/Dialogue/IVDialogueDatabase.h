// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IVAN/IVGenericStructs.h"
#include "IVDialogueDatabase.generated.h"

/**
 * 
 */
UCLASS()
class IVAN_API UIVDialogueDatabase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/* 대화 내용을 담은 데이터베이스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TMap<FName, FDialogueInfo> DialogueEntries;
};
