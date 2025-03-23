// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Enums/IVGenericItemEnums.h"
#include "IVGenericStructs.generated.h"

/**
 * 여러 클래스에서 접근 가능하도록 공통으로 사용되는 구조체들을 모아놓은 헤더파일
 * 현재는 기본 스탯(HP, 스태미나)와 데미지 스텟을 정의
 */

// 캐릭터, 아이템이 공통으로 사용하는 기본 스텟 구조체
USTRUCT(BlueprintType) 
struct FBaseStat
{
    GENERATED_USTRUCT_BODY()

    /* 체력*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float CurrentHP;

    /* 스테미나 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxStamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float CurrentStamina;

	FBaseStat()
	{
		MaxHP = 100.0f;
		CurrentHP = 100.0f;
		MaxStamina = 100.0f;
		CurrentStamina = 100.0f;
	}

	FBaseStat(float InMaxHP, float InCurrentHP, float InMaxStamina, float InCurrentStamina)
	{
		MaxHP = InMaxHP;
		CurrentHP = InCurrentHP;
		MaxStamina = InMaxStamina;
		CurrentStamina = InCurrentStamina;
	}

    /* 연산자 오버로딩 */
    FBaseStat operator+(const FBaseStat& Other) const
    {
        FBaseStat Result;
        Result.MaxHP = MaxHP + Other.MaxHP;
        Result.CurrentHP = CurrentHP + Other.CurrentHP;
        Result.MaxStamina = MaxStamina + Other.MaxStamina;
        Result.CurrentStamina = CurrentStamina + Other.CurrentStamina;
        return Result;
    }

    FBaseStat operator-(const FBaseStat& Other) const
    {
        FBaseStat Result;
        Result.MaxHP = MaxHP - Other.MaxHP;
        Result.CurrentHP = CurrentHP - Other.CurrentHP;
        Result.MaxStamina = MaxStamina - Other.MaxStamina;
        Result.CurrentStamina = CurrentStamina - Other.CurrentStamina;
        return Result;
    }
};

// 데미지 관련 스텟 구조체
USTRUCT(BlueprintType)
struct FBaseDamageStat
{
    GENERATED_USTRUCT_BODY()

    /* 해당 액터가 가진 기본 공격력 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float BaseDamage;

    /* 특수 효과로 인해 일시적으로 증가한 추가 공격력 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float AdditionalDamage;

    /* 방어력. 피격 시 데미지를 경감 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float DamageReduction;

    FBaseDamageStat()
    {
        BaseDamage = 10.0f;
        AdditionalDamage = 0.0f;
        DamageReduction = 0.0f;
    }

    FBaseDamageStat(float InBaseDamage, float InAdditionalDamage, float InDamageReduction)
    {
        BaseDamage = InBaseDamage;
        AdditionalDamage = InAdditionalDamage;
        DamageReduction = InDamageReduction;
    }

    /* 연산자 오버로딩 */
    FBaseDamageStat operator+(const FBaseDamageStat& Other) const
    {
        FBaseDamageStat Result;
        Result.BaseDamage = BaseDamage + Other.BaseDamage;
        Result.AdditionalDamage = AdditionalDamage + Other.AdditionalDamage;
        Result.DamageReduction = DamageReduction + Other.DamageReduction;
        return Result;
    }

    FBaseDamageStat operator-(const FBaseDamageStat& Other) const
    {
        FBaseDamageStat Result;
        Result.BaseDamage = BaseDamage - Other.BaseDamage;
        Result.AdditionalDamage = AdditionalDamage - Other.AdditionalDamage;
        Result.DamageReduction = DamageReduction - Other.DamageReduction;
        return Result;
    }
};


// 아이템 관련 구조체
USTRUCT(BlueprintType)
struct FItemBaseInfo
{
    GENERATED_USTRUCT_BODY()

	/* 아이템 ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemID;

	/* 아이템 타입 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType;

    /* 아이템 이름 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemName;

    /* 아이템 설명 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemDescription;

    /* 아이템을 인벤토리에 쌓아서 저장할수 있는지 여부*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    uint8 bIsStackable : 1;

    /* 아이템 수량 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 ItemCount;

    /* 아이템 아이콘 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* ItemIcon;

    /* 아이템 스태틱 메쉬 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TObjectPtr<UStaticMesh> ItemMesh;

    /* 아이템 기본 스탯 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FBaseStat ItemStat;

    /* 아이템 데미지 스탯 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FBaseDamageStat ItemDamageStat;

    /* 아이템 장착 시 소켓 -> 추후 Enum 사용 고려*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName EquipSocket;

    /* 기본 생성자 */
    FItemBaseInfo()
    {
		ItemID = NAME_None;
		ItemType = EItemType::None;
        ItemName = FString(TEXT("Item"));
        ItemDescription = FString(TEXT("Item Description"));
        bIsStackable = false;
        ItemCount = 0;
        ItemIcon = nullptr;
        ItemMesh = nullptr;
        ItemStat = FBaseStat();
        ItemDamageStat = FBaseDamageStat();
		EquipSocket = NAME_None;
    }

    /* 생성자 */
    FItemBaseInfo(FName InItemID, EItemType InItemType, FString InItemName, FString InItemDescription, bool bInIsStackable, int32 InItemCount, UTexture2D* InItemIcon, UStaticMesh* InItemMesh, FBaseStat InItemStat, FBaseDamageStat InItemDamageStat, FName InEquipSocket)
    {
        ItemID = InItemID;
        ItemType = InItemType;
        ItemName = InItemName;
        ItemDescription = InItemDescription;
        bIsStackable = bInIsStackable;
        ItemCount = InItemCount;
        ItemIcon = InItemIcon;
        ItemMesh = InItemMesh;
        ItemStat = InItemStat;
        ItemDamageStat = InItemDamageStat;
        EquipSocket = InEquipSocket;
    }

	/* == 연산자 오버로딩 */
	bool operator==(const FItemBaseInfo& Other) const
	{
		return ItemID == Other.ItemID;
	}
};

class IVAN_API IVGenericStructs
{
};
