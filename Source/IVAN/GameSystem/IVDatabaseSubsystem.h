// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IVAN/Item/IVItemDatabase.h"
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

	/* 아이템 데이터베이스 접근용 */
	UFUNCTION(BlueprintCallable)
	UIVItemDatabase* GetItemDatabase() const { return ItemDatabase.Get(); }

private:
	/* 아이템 데이터베이스 */
	TObjectPtr<UIVItemDatabase> ItemDatabase;
};
