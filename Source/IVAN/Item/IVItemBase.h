// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IVAN/Enums/IVGenericItemEnums.h"
#include "IVAN/IVGenericStructs.h"
#include "IVItemBase.generated.h"

UCLASS()
class IVAN_API AIVItemBase : public AActor
{
	GENERATED_BODY()
	
// 기본
public:	
	AIVItemBase();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;


// 아이템 정보
public:
	/* 아이템 타입 */
	EItemType GetItemType() const { return ItemType; }
	
	/* 아이템 스텟 */
	FBaseStat GetItemStats() const { return ItemStats; }

protected:
	EItemType ItemType;
	FBaseStat ItemStats;
};
