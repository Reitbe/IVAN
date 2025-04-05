// Fill out your copyright notice in the Description page of Project Settings.


#include "IVInventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "IVAN/UI/IVInventorySlotWidget.h"
#include "IVAN/Item/IVInventoryComponent.h"

void UIVInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 인벤토리 슬롯들을 찾아서 배열에 저장
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

	// 장비 슬롯들을 찾아서 배열에 저장
	if (EquipSlotCanvasPanel)
	{
		EquipSlots.Empty();
		for (UWidget* Widget : EquipSlotCanvasPanel->GetAllChildren())
		{
			if (UIVInventorySlotWidget* SlotWidget = Cast<UIVInventorySlotWidget>(Widget))
			{
				if (SlotWidget != WeaponSlot) // 일반 장비 슬롯의 경우
				{
					EquipSlots.Add(SlotWidget);
					SlotWidget->SetSlotInfo(EInventorySlotType::EquipSlot, EquipSlots.Num() - 1);
					if (DragWidgetClass)
					{
						SlotWidget->SetDragWidgetClass(DragWidgetClass);
					}
				}
				else // 무기 슬롯의 경우
				{
					WeaponSlots.Add(SlotWidget);
					SlotWidget->SetSlotInfo(EInventorySlotType::WeaponSlot, WeaponSlots.Num() - 1);
					if (DragWidgetClass)
					{
						SlotWidget->SetDragWidgetClass(DragWidgetClass);
					}
				}
			}
		}
	}

	// 퀵 슬롯들을 찾아서 배열에 저장
	if (QuickSlotGirdPanel)
	{
		QuickSlots.Empty();
		for (UWidget* Widget : QuickSlotGirdPanel->GetAllChildren())
		{
			if (UIVInventorySlotWidget* SlotWidget = Cast<UIVInventorySlotWidget>(Widget))
			{
				QuickSlots.Add(SlotWidget);
				SlotWidget->SetSlotInfo(EInventorySlotType::QuickSlot, QuickSlots.Num() - 1);
				if (DragWidgetClass)
				{
					SlotWidget->SetDragWidgetClass(DragWidgetClass);
				}
			}
		}
	}
}

void UIVInventoryWidget::UpdateInventorySlots()
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

void UIVInventoryWidget::UpdateQuickSlots()
{
	if (InventoryComponent)
	{
		// 퀵 슬롯들에 아이템 정보 업데이트
		for (int32 i = 0; i < QuickSlots.Num(); i++)
		{
			QuickSlots[i]->SetItemInfo(InventoryComponent->QuickSlots[i]);
		}		
	}
}

void UIVInventoryWidget::UpdateEquipSlots()
{
	if (InventoryComponent)
	{
		// 장비 슬롯들에 아이템 정보 업데이트
		for (int32 i = 0; i < EquipSlots.Num(); i++)
		{
			EquipSlots[i]->SetItemInfo(InventoryComponent->EquipSlots[i]);
		}
	}
}

void UIVInventoryWidget::UpdateWeaponSlot()
{
	if (InventoryComponent)
	{
		// 무기 슬롯에 아이템 정보 업데이트
		for (int32 i = 0; i < WeaponSlots.Num(); i++)
		{
			WeaponSlots[i]->SetItemInfo(InventoryComponent->WeaponSlots[i]);
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

		for (int32 i = 0; i < EquipSlots.Num(); i++)
		{
			EquipSlots[i]->SetInventoryComponent(InventoryComponent);
		}

		for (int32 i = 0; i < QuickSlots.Num(); i++)
		{
			QuickSlots[i]->SetInventoryComponent(InventoryComponent);
		}

		for (int32 i = 0; i < WeaponSlots.Num(); i++)
		{
			WeaponSlots[i]->SetInventoryComponent(InventoryComponent);
		}
	}

}


