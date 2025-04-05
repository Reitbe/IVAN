// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/MulticastDelegateBase.h"
#include "IVAN/IVGenericStructs.h"
#include "IVAN/Enums/IVGenericItemEnums.h"
#include "IVInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventorySlotUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuickSlotUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipSlotUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponSlotUpdated);

class UIVItemDatabase;
class UIVCharacterStatComponent;
class UIVEquipComponent;

/*
* 플레이어 캐릭터의 아이템을 관리하는 인벤토리 컴포넌트.
* 일반 아이템 저장 공간인 인벤토리, HUD에 표시되는 목록인 퀵 슬롯, 장비 탈부착을 위한 장비 슬롯이 존재한다.
* 클릭 및 드래그 앤 드랍을 통해 아이템의 사용, 인벤토리 내 이동, 버리기 등의 기능을 제공한다. 
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IVAN_API UIVInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

// 기본
public:	
	UIVInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;


// 외부 연결
private:
	/* 플레이어 스탯 컴포넌트 */
	TObjectPtr<UIVCharacterStatComponent> CharacterStatComponent;

	/* 플레이어 장비 컴포넌트 */
	TObjectPtr<UIVEquipComponent> EquipComponent;


// 인벤토리 갱신 대리자
public:
	void NotifySlotUpdated(EInventorySlotType SlotType);

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventorySlotUpdated OnInventorySlotUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnQuickSlotUpdated OnQuickSlotUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnEquipSlotUpdated OnEquipSlotUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnWeaponSlotUpdated OnWeaponSlotUpdated;

private:
	/* 대리자 목록*/
	TArray<FMulticastScriptDelegate*> SlotDelegates;


// 인벤토리 저장 공간
public:
	/* 슬롯 타입에 맞는 인벤토리 제공자 */
	TArray<FItemBaseInfo>* GetSlotArray(EInventorySlotType SlotType);

	/* 아이템 데이터베이스 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UIVItemDatabase> ItemDatabase;

	/* 유저 인벤토리 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FItemBaseInfo> InventorySlots;

	/* 퀵 슬롯 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FItemBaseInfo> QuickSlots;

	/* 장비 슬롯 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FItemBaseInfo> EquipSlots;

	/* 무기 슬롯 */ 
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FItemBaseInfo> WeaponSlots;

private:
	/* 슬롯타입-인벤토리 연결 */
	TArray<TArray<FItemBaseInfo>*> SlotTypeToSlotArray;



// 인벤토리 크기 지정
protected:
	/* 인벤토리 최대 크기*/
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Inventory")
	int32 InventorySlotSize;

	/* 퀵 슬롯 최대 크기 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Inventory")
	int32 QuickSlotSize;

	/* 장비 슬롯 최대 크기 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Inventory")
	int32 EquipSlotSize;

	/* 무기 슬롯 최대 크기 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Inventory")
	int32 WeaponSlotSize;


// 인벤토리 내부 기능
public:
	/* 인벤토리에 아이템 추가 - Only 인벤토리 슬롯 */ 
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItemToInventory(const FName& ItemID);

	/* 아이템 ID로 아이템 제거 - 슬롯 공용 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemFromInventoryByItemID(EInventorySlotType SlotType, const FName& ItemID);

	/* 아이템 인덱스로 아이템 제거 - 슬롯 공용*/ 
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemFromInventoryByIndex(EInventorySlotType SlotType, const int32 SlotIndex);

	/* 인벤토리 슬롯간 아이템 이동 */ 
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool SwapInventorySlot(EInventorySlotType FromSlotType, int32 FromSlotIndex, EInventorySlotType ToSlotType, int32 ToSlotIndex);
		
	/* 인벤토리에서 레벨로 아이템 드랍 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool DropItemFromInventoryToLevel(EInventorySlotType SlotType, const int32 SlotIndex);

	/* 인벤토리간 아이템 드래그 드랍 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool DragDropItem(EInventorySlotType FromSlotType, int32 FromSlotIndex, EInventorySlotType ToSlotType, int32 ToSlotIndex);

	/* 아이템 사용 입력 - 타입에 따라 다른 사용 함수 호출 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItemFromSlot(EInventorySlotType SlotType, const int32 SlotIndex);

	/* 소비 아이템 사용 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool ConsumeItem(EInventorySlotType SlotType, const int32 SlotIndex);

	/* 장비 아이템 장착 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool EquipItem(EInventorySlotType FromSlotType, int32 FromSlotIndex, EInventorySlotType ToSlotType, int32 ToSlotIndex);

	/* 인벤토리 초기화 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ResetInventory();

private:
	/* 슬롯 유효성 검사 - 유효 시 슬롯 반환, 아니면 nullptr 반환 */
	TArray<FItemBaseInfo>* IsValidSlot(EInventorySlotType SlotType, const int32 SlotIndex);

	/* 인벤토리 슬롯간 아이템 스왑 가능 여부 검사 */
	bool CanSwapSlot(EInventorySlotType FromSlotType, int32 FromSlotIndex, EInventorySlotType ToSlotType, int32 ToSlotIndex);
};
