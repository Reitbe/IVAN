// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "IVAN/IVGenericStructs.h"
#include "Kismet/GameplayStatics.h"
#include "IVSaveGame.generated.h"

/**
 * 게임 진행 정보를 저장하는 세이브 게임 클래스.
 * 스테이지 정보 / 플레이어의 위치 / 인벤토리 정보를 저장한다.
 */
UCLASS()
class IVAN_API UIVSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/* 생성자 */
	UIVSaveGame();

	/* 스테이지 상태 */
	UPROPERTY(VisibleAnywhere, Category = "Stage")
	EStageState SavedStageState;

	/* 플레이어 위치 */
	UPROPERTY(VisibleAnywhere, Category = "Transform")
	FTransform SavedTransform;

	/* 유저 인벤토리 */
	UPROPERTY(VisibleAnywhere, Category = "Transform")
	TArray<FItemBaseInfo> SavedInventorySlots;

	/* 퀵 슬롯 */
	UPROPERTY(VisibleAnywhere, Category = "Transform")
	TArray<FItemBaseInfo> SavedQuickSlots;

	/* 장비 슬롯 */
	UPROPERTY(VisibleAnywhere, Category = "Transform")
	TArray<FItemBaseInfo> SavedEquipSlots;

	/* 무기 슬롯 */
	UPROPERTY(VisibleAnywhere, Category = "Transform")
	TArray<FItemBaseInfo> SavedWeaponSlots;
	
};
