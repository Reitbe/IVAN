// Fill out your copyright notice in the Description page of Project Settings.


#include "IVInventoryComponent.h"
#include "IVAN/Item/IVItemDatabase.h"
#include "IVAN/GameSystem/IVDatabaseSubsystem.h"

UIVInventoryComponent::UIVInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InventorySlotSize = 20;
	QuickSlotSize = 4;
	EquipSlotSize = 9;

	// 인벤토리 초기화 -> 크기 지정
	InventorySlots.Init(FItemBaseInfo(), InventorySlotSize);
	QuickSlots.Init(FItemBaseInfo(), QuickSlotSize);
	EquipSlots.Init(FItemBaseInfo(), EquipSlotSize);
}

void UIVInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// 아이템 정보는 데이터베이스 서브시스템에서 생성 및 관리한다
	UIVDatabaseSubsystem* DatabaseSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UIVDatabaseSubsystem>();
	if (DatabaseSubsystem)
	{
		ItemDatabase = DatabaseSubsystem->GetItemDatabase();
	}
}

bool UIVInventoryComponent::AddItemToInventory(const FName& ItemID)
{
	bool bAddItemToInventoryComplete = false;

	if (ItemDatabase)
	{
		FItemBaseInfo* ItemBaseInfo = ItemDatabase->GetItemBaseInfo(ItemID);
		if (ItemBaseInfo)
		{
			// 슬롯 전체를 역으로 탐색하며 획득한 아이템과 동일한 아이템이 있는지, 빈 슬롯은 어디인지 확인
			int32 EmptySlotIdx = -1;
			int32 SameItemIdx = -1;
			for (int32 SlotIdx = InventorySlots.Num() - 1; SlotIdx >= 0; SlotIdx--)
			{
				if (InventorySlots[SlotIdx].ItemID == ItemID) // 같은 아이템이 인벤토리에 있음
				{
					SameItemIdx = SlotIdx;
				}
				else if (InventorySlots[SlotIdx].ItemID.IsNone()) // 빈 슬롯
				{
					EmptySlotIdx = SlotIdx;
				}
			}

			// 아이템 삽입
			if (ItemBaseInfo->bIsStackable) // 스택이 가능한 아이템인 경우
			{
				if (SameItemIdx != -1) // 스택O, 인벤토리에 같은 아이템O
				{
					InventorySlots[SameItemIdx].ItemCount++;
					bAddItemToInventoryComplete = true;
				}
				else if(EmptySlotIdx != -1) // 스택O, 인벤토리에 같은 아이템X, 빈 슬롯O
				{
					InventorySlots[EmptySlotIdx] = *ItemBaseInfo;
					InventorySlots[EmptySlotIdx].ItemCount = 1;
					bAddItemToInventoryComplete = true;
				}
				else // 스택O, 인벤토리에 같은 아이템X, 빈 슬롯X -> 삽입 불가
				{
					bAddItemToInventoryComplete = false;
				}
			}
			else //스택이 불가능한 아이탬인 경우
			{
				if (EmptySlotIdx != -1) // 스택X, 빈 슬롯O
				{
					InventorySlots[EmptySlotIdx] = *ItemBaseInfo;
					InventorySlots[EmptySlotIdx].ItemCount = 1;
					bAddItemToInventoryComplete = true;
				}
				else // 스택X, 빈 슬롯X -> 삽입 불가
				{
					bAddItemToInventoryComplete = false;
				}
			}
		}
	}

	if (bAddItemToInventoryComplete) // 인벤토리 갱신
	{
		OnInventorySlotUpdated.Broadcast();
	}
	return bAddItemToInventoryComplete; 
}


bool UIVInventoryComponent::RemoveItemFromInventoryByItemID(const FName& ItemID)
{
	bool bRemoveItemFromInventoryComplete = false;

	if (ItemDatabase)
	{
		FItemBaseInfo* ItemBaseInfo = ItemDatabase->GetItemBaseInfo(ItemID);
		if (ItemBaseInfo)
		{
			// 모든 슬롯을 순회하며 제거할 아이템 인덱스 찾기
			int32 RemoveItemIdx = -1;
			for(int32 SlotIdx = 0; SlotIdx < InventorySlots.Num(); SlotIdx++)
			{
				if (InventorySlots[SlotIdx].ItemID == ItemID)
				{
					RemoveItemIdx = SlotIdx;
					break;
				}
			}

			// 아이템이 존재하는 경우 제거
			if (RemoveItemIdx != -1)
			{
				RemoveItemFromInventoryByIndex(RemoveItemIdx);
				bRemoveItemFromInventoryComplete = true;
			}
			else
			{
				bRemoveItemFromInventoryComplete = false;
			}
		}
	}
	return bRemoveItemFromInventoryComplete;
}

bool UIVInventoryComponent::RemoveItemFromInventoryByIndex(const int32 Index)
{
	// 인덱스 및 인덱스의 내용물 유효성 검사
	if (Index < 0 || Index >= InventorySlots.Num() || InventorySlots[Index].ItemID.IsNone())
	{
		return false;
	}

	// 인덱스 아이템 카운트 확인
	if (InventorySlots[Index].ItemCount > 1) // 스택 가능한 아이템인 경우 감소 처리
	{
		InventorySlots[Index].ItemCount--;
	}
	else // 스택 여부 관계 없이 1개인 경우는 아이템 제거
	{
		InventorySlots[Index] = FItemBaseInfo();
	}

	OnInventorySlotUpdated.Broadcast(); // 인벤토리 갱신
	return true;
}

bool UIVInventoryComponent::SwapInventorySlot(EInventorySlotType FromSlotType, int32 FromSlotIndex, EInventorySlotType ToSlotType, int32 ToSlotIndex)
{
	// 슬롯 유효성 검사
	if (FromSlotIndex < 0 || FromSlotIndex >= InventorySlots.Num() || ToSlotIndex < 0 || ToSlotIndex >= InventorySlots.Num())
	{
		return false;
	}

	// 임시로 인벤토리 슬롯 내부 교환 처리
	FItemBaseInfo TempItem = InventorySlots[FromSlotIndex];
	InventorySlots[FromSlotIndex] = InventorySlots[ToSlotIndex];
	InventorySlots[ToSlotIndex] = TempItem;

	OnInventorySlotUpdated.Broadcast(); // 인벤토리 갱신

	return false;
}


// Called every frame
void UIVInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

