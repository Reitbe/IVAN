// Fill out your copyright notice in the Description page of Project Settings.


#include "IVSaveManagerSubsystem.h"

UIVSaveManagerSubsystem::UIVSaveManagerSubsystem()
{
}

void UIVSaveManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 게임 저장 파일 초기화
	//SaveGameFile = Cast<UIVSaveGame>(UGameplayStatics::CreateSaveGameObject(UIVSaveGame::StaticClass()));
	
	USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(TEXT("DefaultSlot"), 0);
	if (SaveGame)
	{
		SaveGameFile = Cast<UIVSaveGame>(SaveGame); // 세이브 파일이 존재하면 로드
	}
	else
	{
		SaveGameFile = Cast<UIVSaveGame>(UGameplayStatics::CreateSaveGameObject(UIVSaveGame::StaticClass())); // 세이브 파일이 존재하지 않으면 새로 생성
	}
}

void UIVSaveManagerSubsystem::RequestSaveGame()
{
	if (SaveGameFile)
	{
		OnSaveRequested.Broadcast(SaveGameFile); // 각 요소들이 직접 데이터를 덮어쓰도록 요청한다
		UGameplayStatics::SaveGameToSlot(SaveGameFile, TEXT("DefaultSlot"), 0); // 이후 실제로 저장
	}
}

void UIVSaveManagerSubsystem::SaveTransform(const FTransform& Transform)
{
	if (SaveGameFile)
	{
		SaveGameFile->SavedTransform = Transform;
	}
}

EStageState UIVSaveManagerSubsystem::GetStageState() const
{
	return (SaveGameFile) ? SaveGameFile->SavedStageState : EStageState();
}

FTransform UIVSaveManagerSubsystem::GetTransform() const
{
	return (SaveGameFile) ? SaveGameFile->SavedTransform : FTransform();
}

TArray<FItemBaseInfo> UIVSaveManagerSubsystem::GetInventorySlots() const
{
	return (SaveGameFile) ? SaveGameFile->SavedInventorySlots : TArray<FItemBaseInfo>();
}

TArray<FItemBaseInfo> UIVSaveManagerSubsystem::GetQuickSlots() const
{
	return (SaveGameFile) ? SaveGameFile->SavedQuickSlots : TArray<FItemBaseInfo>();
}

TArray<FItemBaseInfo> UIVSaveManagerSubsystem::GetEquipSlots() const
{
	return (SaveGameFile) ? SaveGameFile->SavedEquipSlots : TArray<FItemBaseInfo>();
}

TArray<FItemBaseInfo> UIVSaveManagerSubsystem::GetWeaponSlots() const
{
	return (SaveGameFile) ? SaveGameFile->SavedWeaponSlots : TArray<FItemBaseInfo>();
}
