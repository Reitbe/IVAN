// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IVInventoryBaseWidget.generated.h"

class UIVInventoryWidget;
class UIVInventoryComponent;

/**
 * 인벤토리 위젯이 움직일 수 있는 바탕 캔버스 위젯
 */
UCLASS()
class IVAN_API UIVInventoryBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;


// 플레이어 인벤토리 컴포넌트
public:
	/* 플레이어의 인벤토리 컴포넌트 - HUD에서 세팅 */
	TObjectPtr<UIVInventoryComponent> InventoryComponent;


// 위젯 동작
protected:
	/* 
	* InventoryWidget에는 InventorySlotWidget이 여럿 존재한다.
	* 이 슬롯들은 드래그 앤 드롭으로 옮길 수 있으며, 슬롯을 이 베이스에 드랍했을 경우 아래 함수가 호출된다.
	*/
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


// 위젯 구성 요소
public:
	/* 인벤토리 창 위젯 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UIVInventoryWidget> InventoryWidget;
};
