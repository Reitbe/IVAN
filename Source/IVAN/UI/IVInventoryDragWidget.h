// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IVInventoryDragWidget.generated.h"

class UImage;

/**
 * 아이템 슬롯을 드래그했을 때, 드래그를 따라 이동하는 아이콘 위젯
 */
UCLASS()
class IVAN_API UIVInventoryDragWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	/* 드래그 아이콘 위젯 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIconWidget;
	
	/* 드래그 아이콘 세팅 */
	UFUNCTION(BlueprintCallable, Category = "InventoryDrag")
	void SetDragIcon(UTexture2D* InIconTexture);
	
};
