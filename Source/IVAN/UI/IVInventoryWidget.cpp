// Fill out your copyright notice in the Description page of Project Settings.


#include "IVInventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "IVAN/UI/IVInventorySlotWidget.h"
#include "IVAN/Item/IVInventoryComponent.h"

void UIVInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 그리드 패널에 존재하는 모든 슬롯들을 찾아서 배열에 넣기
	if (InventoryGridPanel)
	{ 
		InventorySlots.Empty();
		for (UWidget* Widget : InventoryGridPanel->GetAllChildren())
		{
			if (UIVInventorySlotWidget* SlotWidget = Cast<UIVInventorySlotWidget>(Widget))
			{
				InventorySlots.Add(SlotWidget);
				SlotWidget->SetSlotInfo(EInventorySlotType::InventorySlot, InventorySlots.Num() - 1);
				if (DragWidgetClass)
				{
					SlotWidget->SetDragWidgetClass(DragWidgetClass);
				}
			}
		}
	}
}

void UIVInventoryWidget::UpdateInventory()
{
	if (InventoryComponent)
	{
		// 인벤토리 슬롯들에 아이템 정보 업데이트
		for (int32 i = 0; i < InventorySlots.Num(); i++)
		{
			InventorySlots[i]->SetItemInfo(InventoryComponent->InventorySlots[i]);
		}
	}
}

void UIVInventoryWidget::InitializeInventorySlots()
{
	if (InventoryComponent)
	{
		for (int32 i = 0; i < InventorySlots.Num(); i++)
		{
			InventorySlots[i]->SetInventoryComponent(InventoryComponent);
		}
	}
}


