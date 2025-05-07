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
}
