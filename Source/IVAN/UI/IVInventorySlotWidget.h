// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IVAN/IVGenericStructs.h"
#include "IVAN/Enums/IVGenericItemEnums.h"
#include "IVInventorySlotWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UIVInventoryDragWidget;
class UIVInventoryComponent;

/**
 * 인벤토리 위젯에 부착되어 아이템의 아이콘과 수량을 나타내는 위젯.
 * 드래그 혹은 더블클릭을 통해 아이템의 이동, 사용, 버리기 등의 동작을 수행한다.
 */
UCLASS()
class IVAN_API UIVInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

// 기본
protected:
	virtual void NativeConstruct() override;


// 슬롯 설정
public:
	/* 드래그 시 마우스를 따라 이동할 위젯 클래스 설정 */
	void SetDragWidgetClass(TSubclassOf<UIVInventoryDragWidget> InDragWidgetClass);

	/* 플레이어의 인벤토리 컴포넌트 접근용 */
	void SetInventoryComponent(UIVInventoryComponent* InInventoryComponent);

	/* 이 위젯의 정보가 어떤 슬롯의 몇 번 인덱스를 반영한 것인지 설정*/
	void SetSlotInfo(EInventorySlotType InSlotType, int32 InSlotIndex);

private:
	/* 드래그 시 생성할 위젯 정보 */
	TSubclassOf<UIVInventoryDragWidget> DragWidgetClass;

	/* 플레이어의 인벤토리 컴포넌트 */
	TObjectPtr<UIVInventoryComponent> InventoryComponent;

	/* 슬롯 타입 - 인벤토리의 어떤 배열에 저장되어있는가 */
	EInventorySlotType InventorySlotType;

	/* 슬롯 인덱스 */
	int32 SlotIndex;


// 슬롯 동작
protected:
	/* 슬롯 버튼 눌림 감지 */
	UFUNCTION()
	void OnItemSlotButtonPressed();

	/* 마우스 입력 - 드래그 진행 - 드래그 종료 */
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	/* 버튼이 눌렸을 때 위젯에 이벤트 전달용 */
	UFUNCTION(BlueprintCallable)
	FEventReply RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);


// 아이템 설정
public:
	/* 아이템 정보 세팅 */
	void SetItemInfo(const FItemBaseInfo& ItemInfo);

private:
	/* 아이템 이미지*/
	TObjectPtr<UTexture2D> ItemIconTexture;


// 위젯 구성 요소
public:
	/* 아이템 아이콘 위젯*/
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIconWidget;

	/* 아이템 수량 위젯 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCountWidget;

	/* 아이템 사용 버튼 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ItemSlotButton;




};
