// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IVQuickSlotWidget.generated.h"

class UIVInventoryComponent;
class UImage;
class UTextBlock;

/**
 * HUD에 부착할 퀵슬롯 위젯.
 * 인벤토리 컴포넌트의 퀵슬롯 정보를 가져와 표시한다
 */
UCLASS()
class IVAN_API UIVQuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
// 기본
protected:
	virtual void NativeConstruct() override;


// 위젯 동작
public:
	/* 퀵슬롯 아이템 이미지 및 수량 업데이트*/
	UFUNCTION()
	void UpdateQuickSlots();

	void SetInventoryComponent(UIVInventoryComponent* NewInventoryComponent) { InventoryComponent = NewInventoryComponent; }

// 위젯 구성 요소
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> FirstQuickSlotImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SecondQuickSlotImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ThirdQuickSlotImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> FourthQuickSlotImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> FirstQuickSlotText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SecondQuickSlotText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ThirdQuickSlotText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> FourthQuickSlotText;

private:
	int32 QuickSlotSize;

	/* 퀵슬롯 이미지 목록 */
	TArray<TObjectPtr<UImage>> QuickSlotImages;
	
	/* 퀵슬롯 텍스트 목록*/
	TArray<TObjectPtr<UTextBlock>> QuickSlotTexts;

	/* 플레이어 인벤토리 컴포넌트 */
	TObjectPtr<UIVInventoryComponent> InventoryComponent;

};
