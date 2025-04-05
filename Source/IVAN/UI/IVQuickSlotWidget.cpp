// Fill out your copyright notice in the Description page of Project Settings.


#include "IVQuickSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "IVAN/Item/IVInventoryComponent.h"

void UIVQuickSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	QuickSlotSize = 4;

	// 이미지 및 텍스트 목록 초기화
	QuickSlotImages.Init(nullptr, QuickSlotSize);
	QuickSlotTexts.Init(nullptr, QuickSlotSize);

	// 이미지 및 텍스트 목록 채우기
	QuickSlotImages[0] = FirstQuickSlotImage;
	QuickSlotImages[1] = SecondQuickSlotImage;
	QuickSlotImages[2] = ThirdQuickSlotImage;
	QuickSlotImages[3] = FourthQuickSlotImage;

	QuickSlotTexts[0] = FirstQuickSlotText;
	QuickSlotTexts[1] = SecondQuickSlotText;
	QuickSlotTexts[2] = ThirdQuickSlotText;
	QuickSlotTexts[3] = FourthQuickSlotText;
}

void UIVQuickSlotWidget::UpdateQuickSlots()
{
	if (InventoryComponent)
	{
		// 퀵 슬롯들에 아이템 정보 업데이트
		for (int32 i = 0; i < QuickSlotSize; i++)
		{
			if (QuickSlotImages[i] && QuickSlotTexts[i])
			{
				// 퀵 슬롯에 아이템이 있을 경우 이미지 및 수량 업데이트
				if (InventoryComponent->QuickSlots.IsValidIndex(i))
				{
					QuickSlotImages[i]->SetVisibility(ESlateVisibility::Visible);
					QuickSlotImages[i]->SetBrushFromTexture(InventoryComponent->QuickSlots[i].ItemIcon);
					QuickSlotTexts[i]->SetText(FText::AsNumber(InventoryComponent->QuickSlots[i].ItemCount));
				}
				// 퀵 슬롯에 아이템이 없을 경우 이미지 및 수량 숨김
				else
				{
					QuickSlotImages[i]->SetVisibility(ESlateVisibility::Hidden);
					QuickSlotTexts[i]->SetText(FText::FromString(""));
				}
			}
		}
	}
}

