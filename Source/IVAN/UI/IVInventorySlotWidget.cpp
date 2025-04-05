// Fill out your copyright notice in the Description page of Project Settings.


#include "IVInventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "IVAN/UI/IVInventoryDragWidget.h"
#include "IVAN/UI/IVDragDropOperation.h"
#include "IVAN/Item/IVInventoryComponent.h"

void UIVInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemSlotButton)
	{
		ItemSlotButton->OnPressed.AddDynamic(this, &UIVInventorySlotWidget::OnItemSlotButtonPressed);
	}
}

void UIVInventorySlotWidget::SetDragWidgetClass(TSubclassOf<UIVInventoryDragWidget> InDragWidgetClass)
{
	DragWidgetClass = InDragWidgetClass;
}

void UIVInventorySlotWidget::OnItemSlotButtonPressed()
{
	if (InventoryComponent)
	{
		InventoryComponent->UseItemFromSlot(InventorySlotType, SlotIndex);
	}
}

FReply UIVInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	FEventReply Reply;
	Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	return Reply.NativeReply;
}

void UIVInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	

	// 드래그 동작
	UIVDragDropOperation* DragDropOperation = Cast<UIVDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UIVDragDropOperation::StaticClass()));
	if (DragDropOperation)
	{
		DragDropOperation->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		DragDropOperation->InventorySlotType = InventorySlotType;
		DragDropOperation->SlotIndex = SlotIndex;
		
		DragDropOperation->Pivot = EDragPivot::MouseDown;

		// 드래그용 위젯 생성
		if (DragWidgetClass)
		{
			UIVInventoryDragWidget* DragWidget = CreateWidget<UIVInventoryDragWidget>(GetWorld(), DragWidgetClass);
			if (DragWidget)
			{
				DragWidget->SetDragIcon(ItemIconTexture);
				DragDropOperation->DefaultDragVisual = DragWidget;
			}
		}

		OutOperation = DragDropOperation;
	}
}

bool UIVInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UIVDragDropOperation* DragDropOperation = Cast<UIVDragDropOperation>(InOperation);
	if (DragDropOperation)
	{
		InventoryComponent->DragDropItem(DragDropOperation->InventorySlotType, DragDropOperation->SlotIndex, InventorySlotType, SlotIndex);
		return true;
	}
	return false;
}

FEventReply UIVInventorySlotWidget::RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return Reply;
}

void UIVInventorySlotWidget::SetInventoryComponent(UIVInventoryComponent* InInventoryComponent)
{
	InventoryComponent = InInventoryComponent;
}

void UIVInventorySlotWidget::SetSlotInfo(EInventorySlotType InSlotType, int32 InSlotIndex)
{
	InventorySlotType = InSlotType;
	SlotIndex = InSlotIndex;
}

void UIVInventorySlotWidget::SetItemInfo(const FItemBaseInfo& ItemInfo)
{
	ItemIconTexture = ItemInfo.ItemIcon;
	ItemIconWidget->SetBrushFromTexture(ItemIconTexture);
	ItemCountWidget->SetText(FText::FromString(FString::FromInt(ItemInfo.ItemCount)));
}
