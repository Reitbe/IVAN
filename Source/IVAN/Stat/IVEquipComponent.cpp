// Fill out your copyright notice in the Description page of Project Settings.


#include "IVEquipComponent.h"
#include "IVAN/Item/IVWeapon.h"
#include "IVAN/Item/IVItemDatabase.h"
#include "IVAN/Item/IVInventoryComponent.h"
#include "IVAN/Interface/IIVEquipInterface.h"
#include "IVAN/Interface/IIVInventoryComponentProvider.h"

UIVEquipComponent::UIVEquipComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	WeaponClass = nullptr;
	WeaponInstance = nullptr;
}

void UIVEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 인벤토리 컴포넌트 연결
	AActor* Owner = GetOwner();
	if (Owner && Owner->Implements<UIIVInventoryComponentProvider>())
	{
		IIIVInventoryComponentProvider* InventoryProvider= Cast<IIIVInventoryComponentProvider>(Owner);
		if (InventoryProvider)
		{
			UIVInventoryComponent* InventoryComponent = InventoryProvider->GetInventoryComponent();
			if (InventoryComponent) // 장비 목록 주소 및 갱신 바인딩
			{
				InventoryComponent->OnEquipSlotUpdated.AddDynamic(this, &UIVEquipComponent::EquipArmors);
				InventoryComponent->OnWeaponSlotUpdated.AddDynamic(this, &UIVEquipComponent::EquipWeapon);
			}
		}
	}

	// 인벤토리가 없더라도 장비컴포넌트에 기본 무기가 지정되어있다면 장착
	if (WeaponClass)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("기본 무기 장착해보자"));
		EquipDefaultWeapon();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("기본 무기 없음"));
	}
}

void UIVEquipComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UIVEquipComponent::EquipWeapon()
{
	// 인벤토리 컴포넌트에 접근하여 무기 가져오기
	AActor* Owner = GetOwner();
	if (Owner)
	{
		// 오너의 인벤토리 컴포넌트에 접근
		UIVInventoryComponent* InventoryComponent = nullptr;
		if (Owner && Owner->Implements<UIIVInventoryComponentProvider>())
		{
			IIIVInventoryComponentProvider* InventoryProvider = Cast<IIIVInventoryComponentProvider>(Owner);
			if (InventoryProvider)
			{
				InventoryComponent = InventoryProvider->GetInventoryComponent();
			}
		}

		// 슬롯에 장착된 무기 클래스를 가져온다.
		FName EquipSocketName = TEXT("WeaponSocket");
		if (InventoryComponent && InventoryComponent->ItemDatabase)
		{
			TArray<FItemBaseInfo>& WeaponList = InventoryComponent->WeaponSlots;
			if (WeaponList.Num() <= 0) return; // 혹시나 초기화 안되어있으면

			FName WeaponID = WeaponList[0].ItemID;
			EquipSocketName = WeaponList[0].EquipSocket;

			if (WeaponID == NAME_None) // 빈 슬롯 -> 무기 해제
			{
				UnEquipWeapon();
				return;
			}
			else if (!InventoryComponent->ItemDatabase->ItemActorMap.Contains(WeaponID)) // DB에 존재하지 않는 ID 
			{
				return;
			}
			else
			{
				WeaponClass = TSubclassOf<AIVWeapon>(InventoryComponent->ItemDatabase->ItemActorMap[WeaponID]);
			}
		}

		// 무기 클래스가 존재한다면 무기를 스폰
		if (WeaponClass)
		{
			if (WeaponInstance) // 기존 무기 액터 제거
			{
				WeaponInstance->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				WeaponInstance->Destroy();
				WeaponInstance = nullptr;
			}
			WeaponInstance = GetWorld()->SpawnActor<AIVWeapon>(WeaponClass, FVector().ZeroVector, FRotator().ZeroRotator);
			WeaponInstance->ApplyEquipSettings();
		}

		// 캐릭터에 무기를 부착한다.
		if (WeaponInstance)
		{
			IIIVEquipInterface* EquipOwner = Cast<IIIVEquipInterface>(GetOwner());
			if (EquipOwner)
			{
				EquipOwner->EquipByInstance(WeaponInstance, EquipSocketName);
			}
		}
	}
}

void UIVEquipComponent::EquipDefaultWeapon()
{
	// 기본으로 지정된 무기 클래스로부터 무기 스폰
	if (WeaponClass)
	{
		IIIVEquipInterface* EquipOwner = Cast<IIIVEquipInterface>(GetOwner());
		if (EquipOwner)
		{
			WeaponInstance = GetWorld()->SpawnActor<AIVWeapon>(WeaponClass, FVector().ZeroVector, FRotator().ZeroRotator);
			WeaponInstance->SetActorLocation(GetOwner()->GetActorLocation()+FVector(0.0f, 0.0f, 100.0f));
			WeaponInstance->ApplyEquipSettings();

			if (WeaponInstance)
			{
				FName EquipSocketName = WeaponInstance->ItemInfo.EquipSocket;
				EquipOwner->EquipByInstance(WeaponInstance, EquipSocketName);
			}
		}
	}
}

void UIVEquipComponent::DropWeapon()
{
	// 무기 물리	시뮬레이션 활성화
	if (WeaponInstance)
	{
		WeaponInstance->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		WeaponInstance->DropItem();
	}
}

void UIVEquipComponent::UnEquipWeapon()
{
	// 손에 들고있는 무기 제거
	if (WeaponInstance)
	{
		WeaponInstance->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		WeaponInstance->Destroy();
		WeaponInstance = nullptr;
	}

	IIIVEquipInterface* EquipOwner = Cast<IIIVEquipInterface>(GetOwner());
	if (EquipOwner)
	{
		EquipOwner->UnEquipWeapon();
	}
}


void UIVEquipComponent::EquipArmors()
{
	AActor* Owner = GetOwner();
	if (Owner)
	{
		// 인벤토리 컴포넌트 접근
		UIVInventoryComponent* InventoryComponent = nullptr;
		if (Owner && Owner->Implements<UIIVInventoryComponentProvider>())
		{
			IIIVInventoryComponentProvider* InventoryProvider = Cast<IIIVInventoryComponentProvider>(Owner);
			if (InventoryProvider)
			{
				InventoryComponent = InventoryProvider->GetInventoryComponent();
			}
		}

		// 캐릭터에서 스켈레탈 메쉬 
		IIIVEquipInterface* MeshProvider = nullptr;
		if (Owner && Owner->Implements<UIIVEquipInterface>())
		{
			MeshProvider = Cast<IIIVEquipInterface>(Owner);
		}

		if (InventoryComponent && MeshProvider)
		{
			TArray<FItemBaseInfo>& EquipList = InventoryComponent->EquipSlots;
			TArray<USkeletalMeshComponent*>& EquipMeshes = MeshProvider->GetEquipMeshArray();
			
			// 장비 메쉬 목록을 초기화한다.
			for (USkeletalMeshComponent* Mesh : EquipMeshes)
			{
				Mesh->SetSkeletalMeshAsset(nullptr);
			}

			// 장비 목록을 순회하며 장비 메쉬에 부착한다.
			for (int32 i = 0; i < EquipList.Num(); i++)
			{
				// 장비 메쉬가 존재하고, 장비 목록에 아이템이 존재한다면
				if (EquipMeshes.IsValidIndex(i) && EquipList.IsValidIndex(i))
				{
					// 장비 메쉬에 아이템을 부착한다.
					USkeletalMesh* Mesh = EquipList[i].ItemSkeletalMesh;
					if (Mesh)
					{
						EquipMeshes[i]->SetSkeletalMeshAsset(Mesh);
					}
				}
			}
		}
	}
}
