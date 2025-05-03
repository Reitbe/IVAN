// Fill out your copyright notice in the Description page of Project Settings.


#include "IVInventoryComponent.h"
#include "IVAN/Item/IVItemDatabase.h"
#include "IVAN/GameSystem/IVDatabaseSubsystem.h"
#include "IVAN/Stat/IVCharacterStatComponent.h"
#include "IVAN/Interface/IIVCharacterComponentProvider.h"
#include "IVAN/Interface/IIVEquipInterface.h"

UIVInventoryComponent::UIVInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InventorySlotSize = 20;
	QuickSlotSize = 4;
	EquipSlotSize = 7; // 무기 슬롯 하나 빠짐
	WeaponSlotSize = 1;

	// 인벤토리 초기화 -> 크기 지정
	InventorySlots.Init(FItemBaseInfo(), InventorySlotSize);
	QuickSlots.Init(FItemBaseInfo(), QuickSlotSize);
	EquipSlots.Init(FItemBaseInfo(), EquipSlotSize);
	WeaponSlots.Init(FItemBaseInfo(), WeaponSlotSize);

	// 슬롯 타입에 따른 슬롯 배열 초기화
	SlotTypeToSlotArray.Init(nullptr, static_cast<int32>(EInventorySlotType::MAX));
	SlotTypeToSlotArray[static_cast<int32>(EInventorySlotType::None)] = nullptr;
	SlotTypeToSlotArray[static_cast<int32>(EInventorySlotType::QuickSlot)] = &QuickSlots;
	SlotTypeToSlotArray[static_cast<int32>(EInventorySlotType::EquipSlot)] = &EquipSlots;
	SlotTypeToSlotArray[static_cast<int32>(EInventorySlotType::WeaponSlot)] = &WeaponSlots;
	SlotTypeToSlotArray[static_cast<int32>(EInventorySlotType::InventorySlot)] = &InventorySlots;

	// 인벤토리 갱신 대리자 초기화
	SlotDelegates.Init(nullptr, static_cast<int32>(EInventorySlotType::MAX));
	SlotDelegates[static_cast<int32>(EInventorySlotType::None)] = nullptr;
	SlotDelegates[static_cast<int32>(EInventorySlotType::QuickSlot)] = &OnQuickSlotUpdated;
	SlotDelegates[static_cast<int32>(EInventorySlotType::EquipSlot)] = &OnEquipSlotUpdated;
	SlotDelegates[static_cast<int32>(EInventorySlotType::WeaponSlot)] = &OnWeaponSlotUpdated;
	SlotDelegates[static_cast<int32>(EInventorySlotType::InventorySlot)] = &OnInventorySlotUpdated;

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

	// 플레이어 스탯 컴포넌트 연결
	AActor* Owner = GetOwner();
	if (Owner && Owner->Implements<UIIVCharacterComponentProvider>())
	{
		IIIVCharacterComponentProvider* Provider = Cast<IIIVCharacterComponentProvider>(Owner);
		CharacterStatComponent = Provider->GetCharacterStatComponent();
	}

	// 플레이어 장비 컴포넌트 연결
	if (Owner && Owner->Implements<UIIVEquipInterface>())
	{
		IIIVEquipInterface* EquipOwner = Cast<IIIVEquipInterface>(Owner);
		EquipComponent = EquipOwner->GetEquipComponent();
	}
}

void UIVInventoryComponent::NotifySlotUpdated(EInventorySlotType SlotType)
{
	int32 Index = static_cast<int32>(SlotType);
	if (Index >= 0 && Index < SlotDelegates.Num() && SlotDelegates[Index])
	{
		// 대리자 호출
		SlotDelegates[Index]->ProcessMulticastDelegate<UObject>(nullptr);
	}
}

TArray<FItemBaseInfo>* UIVInventoryComponent::GetSlotArray(EInventorySlotType SlotType)
{
	int32 Index = static_cast<int32>(SlotType);
	if (Index >= 0 && Index < SlotTypeToSlotArray.Num())
	{
		return SlotTypeToSlotArray[Index];
	}
	return nullptr;
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


bool UIVInventoryComponent::RemoveItemFromInventoryByItemID(EInventorySlotType SlotType, const FName& ItemID)
{
	bool bRemoveItemFromInventoryComplete = false;

	TArray<FItemBaseInfo>* SlotArray = GetSlotArray(SlotType);
	if (SlotArray)
	{
		// 모든 슬롯을 순회하며 제거할 아이템 인덱스 찾기
		int32 RemoveItemIdx = -1;
		for (int32 SlotIdx = 0; SlotIdx < (*SlotArray).Num(); SlotIdx++)
		{
			if ((*SlotArray)[SlotIdx].ItemID == ItemID)
			{
				RemoveItemIdx = SlotIdx;
				break;
			}
		}

		// 아이템이 존재하는 경우 제거
		if (RemoveItemIdx != -1)
		{
			RemoveItemFromInventoryByIndex(SlotType, RemoveItemIdx);
			bRemoveItemFromInventoryComplete = true;
		}
		else
		{
			bRemoveItemFromInventoryComplete = false;
		}
	}
	return bRemoveItemFromInventoryComplete;
}

bool UIVInventoryComponent::RemoveItemFromInventoryByIndex(EInventorySlotType SlotType, const int32 SlotIndex)
{
	// 슬롯 유효성 검사
	TArray<FItemBaseInfo>* SlotArray = IsValidSlot(SlotType, SlotIndex);
	if (!SlotArray)
	{
		return false;
	}

	// 아이템 정보 유효성 검사
	FItemBaseInfo ItemBaseInfo = (*SlotArray)[SlotIndex];
	if (ItemBaseInfo.ItemID.IsNone())
	{
		return false;
	}
	
	// 인덱스 아이템 카운트 확인
	if ((*SlotArray)[SlotIndex].ItemCount > 1) // 스택 가능한 아이템인 경우 감소 처리
	{
		(*SlotArray)[SlotIndex].ItemCount--;
	}
	else // 스택 여부 관계 없이 1개인 경우는 아이템 제거
	{
		(*SlotArray)[SlotIndex] = FItemBaseInfo();
	}

	// 인벤토리 갱신
	NotifySlotUpdated(SlotType);
	return true;
}

FItemBaseInfo UIVInventoryComponent::GetItemInfoFromInventoryByItemID(const FName& ItemID)
{
	// 모든 타입의 모든 슬롯을 순회하며 해당 아이템이 있는지 확인
	for (int32 SlotTypeIdx = 0; SlotTypeIdx < static_cast<int32>(EInventorySlotType::MAX); SlotTypeIdx++)
	{
		TArray<FItemBaseInfo>* SlotArray = GetSlotArray(static_cast<EInventorySlotType>(SlotTypeIdx)); // 슬롯 배열 결정
		if (SlotArray)
		{
			for (int32 SlotIdx = 0; SlotIdx < (*SlotArray).Num(); SlotIdx++) // 슬롯 배열 순회
			{
				if ((*SlotArray)[SlotIdx].ItemID == ItemID)
				{
					return (*SlotArray)[SlotIdx];
				}
			}
		}
	}
	return FItemBaseInfo(); // 아이템이 없는 경우 빈 아이템 정보 반환
}

bool UIVInventoryComponent::SwapInventorySlot(EInventorySlotType FromSlotType, int32 FromSlotIndex, EInventorySlotType ToSlotType, int32 ToSlotIndex)
{
	TArray<FItemBaseInfo>* FromSlotArray = IsValidSlot(FromSlotType, FromSlotIndex);
	TArray<FItemBaseInfo>* ToSlotArray = IsValidSlot(ToSlotType, ToSlotIndex);

	// 슬롯 유효성 검사
	if(!FromSlotArray || !ToSlotArray)
	{
		return false;
	}

	// 스왑할 슬롯과 아이템 타입 호환 검사
	if (!CanSwapSlot(FromSlotType, FromSlotIndex, ToSlotType, ToSlotIndex))
	{
		return false;
	}

	// 슬롯간 교환 처리
	FItemBaseInfo TempItem = (*FromSlotArray)[FromSlotIndex];
	(*FromSlotArray)[FromSlotIndex] = (*ToSlotArray)[ToSlotIndex];
	(*ToSlotArray)[ToSlotIndex] = TempItem;

	// 슬롯에 맞는 인벤토리 갱신
	if (FromSlotType != ToSlotType)
	{
		NotifySlotUpdated(FromSlotType);
		NotifySlotUpdated(ToSlotType);
	}
	else
	{
		NotifySlotUpdated(FromSlotType);
	}

	return false;
}

bool UIVInventoryComponent::DropItemFromInventoryToLevel(EInventorySlotType SlotType, const int32 SlotIndex)
{
	// 슬롯 유효성 검사
	TArray<FItemBaseInfo>* SlotArray = IsValidSlot(SlotType, SlotIndex);
	if (!SlotArray)
	{
		return false;
	}

	// 아이템 정보 유효성 검사
	FItemBaseInfo ItemBaseInfo = (*SlotArray)[SlotIndex];
	if (ItemBaseInfo.ItemID.IsNone())
	{
		return false;
	}

	if (ItemDatabase->ItemActorMap.Find(ItemBaseInfo.ItemID))
	{
		// 아이템 액터 스폰 & 드랍
		AIVItemBase* ItemActor = GetWorld()->SpawnActor<AIVItemBase>(ItemDatabase->ItemActorMap[ItemBaseInfo.ItemID]);
		if (ItemActor)
		{
			ItemActor->InitializeItem(ItemBaseInfo, ItemBaseInfo.ItemCount); // 아이템 수량 보존
			FVector DropLocation = GetOwner()->GetActorLocation() + FVector(100.0f, 0.0f, 0.0f);
			ItemActor->SetActorLocation(DropLocation);
			ItemActor->DropItem();
		}
		else
		{
			return false;
		}
	}

	// 인벤토리에서 아이템 제거
	(*SlotArray)[SlotIndex] = FItemBaseInfo();

	// 슬롯에 맞는 인벤토리 갱신
	NotifySlotUpdated(SlotType);
	return true;
}

bool UIVInventoryComponent::DragDropItem(EInventorySlotType FromSlotType, int32 FromSlotIndex, EInventorySlotType ToSlotType, int32 ToSlotIndex)
{
	TArray<FItemBaseInfo>* FromSlotArray = IsValidSlot(FromSlotType, FromSlotIndex);
	TArray<FItemBaseInfo>* ToSlotArray = IsValidSlot(ToSlotType, ToSlotIndex);

	// 슬롯 유효성 검사
	if (!FromSlotArray || !ToSlotArray)
	{
		return false;
	}

	// 동일 슬롯 타입에서의 이동은 바로 스왑 처리
	if (FromSlotType == ToSlotType)
	{
		return SwapInventorySlot(FromSlotType, FromSlotIndex, ToSlotType, ToSlotIndex);
	}

	// 서로 다른 슬롯 타입간 스왑이 가능한 경우
	if (CanSwapSlot(FromSlotType, FromSlotIndex, ToSlotType, ToSlotIndex))
	{
		// 일반 인벤토리 슬롯과 퀵 슬롯간 이동은 스왑 처리
		if (FromSlotType == EInventorySlotType::QuickSlot || ToSlotType == EInventorySlotType::QuickSlot)
		{
			return SwapInventorySlot(FromSlotType, FromSlotIndex, ToSlotType, ToSlotIndex);
		}
		// 일반 인벤토리 슬롯과 장비 슬롯간 이동은 장비 처리. anrl 무기 슬롯도 동일하다.
		else if (FromSlotType == EInventorySlotType::EquipSlot || ToSlotType == EInventorySlotType::EquipSlot)
		{
			return EquipItem(FromSlotType, FromSlotIndex, ToSlotType, ToSlotIndex);
		}
		else if (FromSlotType == EInventorySlotType::WeaponSlot || ToSlotType == EInventorySlotType::WeaponSlot)
		{
			return EquipItem(FromSlotType, FromSlotIndex, ToSlotType, ToSlotIndex);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool UIVInventoryComponent::UseItemFromSlot(EInventorySlotType SlotType, const int32 SlotIndex)
{
	// 사용할 슬롯 유효성 검사
	TArray<FItemBaseInfo>* SlotArray = IsValidSlot(SlotType, SlotIndex);
	if(!SlotArray)
	{
		return false;
	}

	// 사용할 아이템 정보 유효성 검사
	FItemBaseInfo ItemBaseInfo = (*SlotArray)[SlotIndex];
	if (ItemBaseInfo.ItemID.IsNone())
	{
		return false;
	}

	EItemType ItemType = ItemBaseInfo.ItemType; // 아이템 타입에 따른 동작 분리
	
	// 소비 아이템인 경우 - 스탯 적용 & 인벤토리에서 삭제
	if (ItemType == EItemType::Consumable) 
	{
		return ConsumeItem(SlotType, SlotIndex);
	}
	// 무기 및 장비아이템인 경우 - 스왑 & 기존 장비 해제 & 새 장비 등록
	else if (ItemType == EItemType::Armor || ItemType == EItemType::Weapon) 
	{
		// 목적 배열에 빈 슬롯이 있는지 확인, 빈 슬롯이 없다면 첫번째 슬롯으로 대체
		TArray<FItemBaseInfo>* DestSlotArray = (ItemType == EItemType::Armor) ? &EquipSlots : &WeaponSlots;
		int32 DestSlotIndex = 0;
		for (int32 SlotIdx = 0; SlotIdx < DestSlotArray->Num(); SlotIdx++)
		{
			if ((*DestSlotArray)[SlotIdx].ItemID.IsNone()) // 비어있는 슬롯 번호 구하기
			{
				DestSlotIndex = SlotIdx;
				break;
			}
		}

		// 장비 장착
		EInventorySlotType DestSlotType = (ItemType == EItemType::Armor) ? EInventorySlotType::EquipSlot : EInventorySlotType::WeaponSlot;
		return EquipItem(SlotType, SlotIndex, DestSlotType, DestSlotIndex);
	}
	else
	{
		return false;
	}
}

bool UIVInventoryComponent::ConsumeItem(EInventorySlotType SlotType, const int32 SlotIndex)
{
	/* 슬롯과 아이템 정보는 이미 검사된 상태 */

	if (CharacterStatComponent)
	{
		FItemBaseInfo ItemBaseInfo = (*GetSlotArray(SlotType))[SlotIndex]; // 사용할 아이템의 스탯 정보 획득
		CharacterStatComponent->UseConsumableItem(ItemBaseInfo.ItemStat, ItemBaseInfo.ItemDamageStat); // 아이템 스탯 적용

		if (RemoveItemFromInventoryByIndex(SlotType, SlotIndex)) // 인벤토리에서 아이템 제거
		{ 
			NotifySlotUpdated(SlotType); // 성공시 인벤토리 갱신
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool UIVInventoryComponent::EquipItem(EInventorySlotType FromSlotType, int32 FromSlotIndex, EInventorySlotType ToSlotType, int32 ToSlotIndex)
{
	/* 슬롯과 아이템 정보는 이미 검사된 상태 */

	// 스왑을 먼저 처리한다. 스왑이 성공한다면 스탯 작업을 처리한다. 
	if (SwapInventorySlot(FromSlotType, FromSlotIndex, ToSlotType, ToSlotIndex))
	{
		if (CharacterStatComponent)
		{
			FItemBaseInfo EquipItemInfo = (*GetSlotArray(ToSlotType))[ToSlotIndex]; // 장착한 아이템의 스탯 획득
			CharacterStatComponent->EquipItem(EquipItemInfo.ItemStat, EquipItemInfo.ItemDamageStat); // 아이템 스탯 적용

			FItemBaseInfo UnequipItemInfo = (*GetSlotArray(FromSlotType))[FromSlotIndex]; // 해제한 아이템의 스탯 획득
			CharacterStatComponent->UnequipItem(UnequipItemInfo.ItemStat, UnequipItemInfo.ItemDamageStat); // 아이템 스탯 해제
		}

		return true; // 인벤토리 갱신은 SwapInventorySlot에서 처리함.
	}
	else
	{
		return false;
	}
}

void UIVInventoryComponent::ResetInventory()
{
	// 모든 슬롯 타입에 대하여 슬롯을 기본값으로 지정
	for (int32 TypeIdx = 0; TypeIdx < SlotTypeToSlotArray.Num(); TypeIdx++)
	{
		if (SlotTypeToSlotArray[TypeIdx])
		{
			for (int32 SlotIdx = 0; SlotIdx < (*SlotTypeToSlotArray[TypeIdx]).Num(); SlotIdx++)
			{
				(*SlotTypeToSlotArray[TypeIdx])[SlotIdx] = FItemBaseInfo(); // 슬롯 초기화
			}
		}
	}

	// 모든 대리자 호출
	for (int32 TypeIdx = 0; TypeIdx < SlotDelegates.Num(); TypeIdx++)
	{
		if (SlotDelegates[TypeIdx])
		{
			SlotDelegates[TypeIdx]->ProcessMulticastDelegate<UObject>(nullptr);
		}
	}
}


TArray<FItemBaseInfo>* UIVInventoryComponent::IsValidSlot(EInventorySlotType SlotType, const int32 SlotIndex)
{
	TArray<FItemBaseInfo>* SlotArray = GetSlotArray(SlotType);
	if (SlotArray && SlotIndex >= 0 && SlotIndex < SlotArray->Num())
	{
		return SlotArray;
	}
	return nullptr;
}

bool UIVInventoryComponent::CanSwapSlot(EInventorySlotType FromSlotType, int32 FromSlotIndex, EInventorySlotType ToSlotType, int32 ToSlotIndex)
{
	/* 슬롯과 아이템 정보는 이미 검사된 상태 */

	bool CanAtoB = false; // From -> To 아이템 이동 가능 여부
	bool CanBtoA = false; // To -> From 아이템 이동 가능 여부

	// From의 아이템을 To 슬롯으로 넘길 수 있는가? (A->B)
	EItemType FromItemType = (*GetSlotArray(FromSlotType))[FromSlotIndex].ItemType;
	if (FromItemType == EItemType::None) CanAtoB = true; // FromItemType이 None인 경우는 어느 슬롯이든 통과
	if (ToSlotType == EInventorySlotType::InventorySlot) CanAtoB = true; // 인벤토리 슬롯으로는 모든 아이템 이동 가능
	if (ToSlotType == EInventorySlotType::WeaponSlot && FromItemType == EItemType::Weapon) CanAtoB = true; 
	if (ToSlotType == EInventorySlotType::EquipSlot && FromItemType == EItemType::Armor) CanAtoB = true;
	if (ToSlotType == EInventorySlotType::QuickSlot && FromItemType == EItemType::Consumable) CanAtoB = true;

	// To의 아이템을 From 슬롯으로 넘길 수 있는가? (B->A)
	EItemType ToItemType = (*GetSlotArray(ToSlotType))[ToSlotIndex].ItemType;
	if (ToItemType == EItemType::None) CanBtoA = true; // ToItemType이 None인 경우는 어느 슬롯이든 통과
	if (FromSlotType == EInventorySlotType::InventorySlot) CanBtoA = true; // 인벤토리 슬롯으로는 모든 아이템 이동 가능
	if (FromSlotType == EInventorySlotType::WeaponSlot && ToItemType == EItemType::Weapon) CanBtoA = true;
	if (FromSlotType == EInventorySlotType::EquipSlot && ToItemType == EItemType::Armor) CanBtoA = true;
	if (FromSlotType == EInventorySlotType::QuickSlot && ToItemType == EItemType::Consumable) CanBtoA = true;

	return (CanAtoB && CanBtoA); // 양방향으로 이동 가능해야 스왑 가능
}


// Called every frame
void UIVInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

