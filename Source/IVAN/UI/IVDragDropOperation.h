// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "IVAN/Enums/IVGenericItemEnums.h"
#include "IVDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class IVAN_API UIVDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	/* 아이템 슬롯 타입 */
	EInventorySlotType InventorySlotType;

	/* 슬롯 인덱스 */
	int32 SlotIndex;

	/* 드래그 오프셋 */
	FVector2D DragOffset;

};
