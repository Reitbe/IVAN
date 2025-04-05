// Fill out your copyright notice in the Description page of Project Settings.


#include "IVInventoryBaseWidget.h"
#include "IVAN/Item/IVInventoryComponent.h"
#include "IVAN/UI/IVDragDropOperation.h"

void UIVInventoryBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UIVInventoryBaseWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UIVDragDropOperation* DragDropOperation = Cast<UIVDragDropOperation>(InOperation);
	if (DragDropOperation && InventoryComponent)
	{
		InventoryComponent->DropItemFromInventoryToLevel(DragDropOperation->InventorySlotType, DragDropOperation->SlotIndex);
		return true;
	}
	return false;
}
