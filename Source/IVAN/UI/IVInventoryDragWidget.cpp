// Fill out your copyright notice in the Description page of Project Settings.


#include "IVInventoryDragWidget.h"
#include "Components/Image.h"

void UIVInventoryDragWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UIVInventoryDragWidget::SetDragIcon(UTexture2D* InIconTexture)
{
	ItemIconWidget->SetBrushFromTexture(InIconTexture);
}
