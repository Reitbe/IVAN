// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IVInventoryWidget.generated.h"

class UIVInventoryComponent;
class UIVInventorySlotWidget;
class UIVInventoryDragWidget;
class UUniformGridPanel;
class UCanvasPanel;

/**
 * 인벤토리를 구성하는 인벤토리 베이스 - 인벤토리 위젯 - 인벤토리 슬롯 중 중앙을 담당한다.
 * 캔버스 위에서 이동 가능한 창이며 여러 인벤토리 슬롯들을 보유하는 위젯이다.
 * 창 이동은 블루프린트로 구현했다.
 */
UCLASS()
class IVAN_API UIVInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
// 기본
protected:
	virtual void NativeConstruct() override;


// 인벤토리 세팅
public:
	/* 인벤토리 슬롯 갱신 */
	UFUNCTION()
	void UpdateInventorySlots();

	/* 퀵 슬롯 갱신 */
	UFUNCTION()
	void UpdateQuickSlots();

	/* 장비 슬롯 갱신 */
	UFUNCTION()
	void UpdateEquipSlots();

	/* 무기 슬롯 갱신 */
	UFUNCTION()
	void UpdateWeaponSlot();

	/* 인벤토리 슬롯들에 드래그 위젯 클래스 세팅 */
	void InitializeInventorySlots();

	/* 플레이어의 인벤토리 컴포넌트 - HUD에서 세팅 */
	TObjectPtr<UIVInventoryComponent> InventoryComponent;

	/* 인벤토리 슬롯 드래그 시 생성할 드래그 위젯 클래스 - BP에서 세팅 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryDrag")
	TSubclassOf<UIVInventoryDragWidget> DragWidgetClass;


// 인벤토리 슬롯 목록
public:
	/* 인벤토리 위젯에 부착된 인벤토리 슬롯 목록 */ 
	TArray<UIVInventorySlotWidget*> InventorySlots;

	/* 인벤토리 위젯에 부착된 장비 슬롯 목록 */
	TArray<UIVInventorySlotWidget*> EquipSlots;

	/* 인벤토리 위젯에 부착된 퀵 슬롯 목록 */
	TArray<UIVInventorySlotWidget*> QuickSlots;

	/* 인벤토리 위젯에 부착된 무기 슬롯 목록(사실 단일 요소임)*/
	TArray<UIVInventorySlotWidget*> WeaponSlots;

// 위젯 구성 요소
public:
	/* 인벤토리 슬롯들을 배치할 그리드 패널 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGridPanel;

	/* 퀵 슬롯들을 배치할 그리드 패널 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> QuickSlotGirdPanel;

	/* 장비 슬롯들을 배치할 캔버스 패널 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> EquipSlotCanvasPanel;

	/* 무기 장착용 슬롯*/
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UIVInventorySlotWidget> WeaponSlot;

};
