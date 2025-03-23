// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IVAN/IVGenericStructs.h"
#include "IVAN/Enums/IVGenericItemEnums.h"
#include "IVInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventorySlotUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuickSlotUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipSlotUpdated);

class UIVItemDatabase;

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


// 인벤토리 갱신 대리자
public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventorySlotUpdated OnInventorySlotUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnQuickSlotUpdated OnQuickSlotUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnEquipSlotUpdated OnEquipSlotUpdated;


// 인벤토리 저장 공간
public:
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


// 인벤토리 기능
public:
	/* 인벤토리에 아이템 추가 */ 
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItemToInventory(const FName& ItemID);

	/* 인벤토리에서 아이템 ID로 아이템 제거 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemFromInventoryByItemID(const FName& ItemID);

	/* 인벤토리에서 아이템 인덱스로 아이템 제거 */ 
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemFromInventoryByIndex(const int32 Index);

	/* 인벤토리 슬롯간 아이템 이동 */ 
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool SwapInventorySlot(EInventorySlotType FromSlotType, int32 FromSlotIndex, EInventorySlotType ToSlotType, int32 ToSlotIndex);
		
};
