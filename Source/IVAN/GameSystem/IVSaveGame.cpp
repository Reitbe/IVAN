// Fill out your copyright notice in the Description page of Project Settings.


#include "IVSaveGame.h"

UIVSaveGame::UIVSaveGame()
{
	SavedStageState = EStageState::FirstStage;
	SavedTransform = FTransform();
	SavedInventorySlots.Empty();
	SavedQuickSlots.Empty();
	SavedEquipSlots.Empty();
	SavedWeaponSlots.Empty();

	// 인벤토리 초기화 -> 크기 지정(임시 고정 값)
	SavedInventorySlots.Init(FItemBaseInfo(), 20);
	SavedQuickSlots.Init(FItemBaseInfo(), 4);
	SavedEquipSlots.Init(FItemBaseInfo(), 7);
	SavedWeaponSlots.Init(FItemBaseInfo(), 1);
}
