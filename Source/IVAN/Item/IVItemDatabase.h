// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IVAN/IVGenericStructs.h"
#include "IVAN/Item/IVItemBase.h"
#include "IVItemDatabase.generated.h"

/**
 * 모든 아이템을 한 곳에서 관리하기 위한 아이템 데이터베이스.
 * 데이터베이스 내용은 데이터 에셋으로 저장되며, DatabaseSubsystem에서 관리된다.
 */
UCLASS()
class IVAN_API UIVItemDatabase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/* 데이터베이스에 저장된 아이템의 세부 정보들 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
	TArray<FItemBaseInfo> Items;

	/* 아이템 ID와 생성할 액터 블루프린트를 매칭해둔 목록. 아이템 스폰에 사용된다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
	TMap<FName, TSubclassOf<AIVItemBase>> ItemActorMap;

	/* 아이템 ID를 기반으로 세부 정보를 반환 */
	FItemBaseInfo* GetItemBaseInfo(const FName& ItemID);
};
