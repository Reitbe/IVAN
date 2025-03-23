// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 아이템 및 하위 클래스들의 열거형 모음.
 */

// 아이템 타입
UENUM(BlueprintType)
enum class EItemType : uint8
{
	None            UMETA(DisplayName = "None"),
	Consumable      UMETA(DisplayName = "Consumable"),
	Weapon          UMETA(DisplayName = "Weapon"),
	Armor           UMETA(DisplayName = "Armor"),
};

// 방어구 장착 위치. 추후 FName으로 변경할 수도 있음.
UENUM(BlueprintType)
enum class EArmorSlot : uint8
{
    Helm UMETA(DisplayName = "Helm"),
    Legs UMETA(DisplayName = "Legs"),
    Shoulder UMETA(DisplayName = "Shoulder"),
    Feet UMETA(DisplayName = "Feet"),
    Head UMETA(DisplayName = "Head"),
    Hands UMETA(DisplayName = "Hands"),
    Necklace UMETA(DisplayName = "Necklace"),
    Belt UMETA(DisplayName = "Belt"),
    Cloak UMETA(DisplayName = "Cloak")
};

// 인벤토리에서 아이템이 위치할 수 있는 슬롯 타입
UENUM(BlueprintType)
enum class EInventorySlotType : uint8
{
	None            UMETA(DisplayName = "None"),
	QuickSlot       UMETA(DisplayName = "QuickSlot"),
	EquipSlot       UMETA(DisplayName = "EquipSlot"),
	InventorySlot   UMETA(DisplayName = "InventorySlot"),
};

class IVAN_API IVGenericItemEnums
{
};
