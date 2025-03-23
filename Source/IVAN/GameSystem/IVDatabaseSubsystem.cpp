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
}
