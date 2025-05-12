// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Enums/IVCharacterStateEnums.h"
#include "IVAN/Enums/IVGenericItemEnums.h"
#include "IVAN/Enums/IVGameProgressEnums.h"
#include "IVGenericStructs.generated.h"

/**
 * 여러 클래스에서 접근 가능하도록 공통으로 사용되는 구조체들을 모아놓은 헤더파일
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
		MaxHP = 0.0f;
		CurrentHP = 0.0f;
		MaxStamina = 0.0f;
		CurrentStamina = 0.0f;
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
		Result.CurrentHP = FMath::Clamp(CurrentHP + Other.CurrentHP, 0.0f, MaxHP);
        Result.MaxStamina = MaxStamina + Other.MaxStamina;
        Result.CurrentStamina = FMath::Clamp(CurrentStamina + Other.CurrentStamina, 0.0f, MaxStamina);
        return Result;
    }

    FBaseStat operator-(const FBaseStat& Other) const
    {
        FBaseStat Result;
        Result.MaxHP = MaxHP - Other.MaxHP;
        Result.CurrentHP = FMath::Clamp(CurrentHP - Other.CurrentHP, 0.0f, MaxHP);
        Result.MaxStamina = MaxStamina - Other.MaxStamina;
        Result.CurrentStamina = FMath::Clamp(CurrentStamina - Other.CurrentStamina, 0.0f, MaxStamina);
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
        BaseDamage = 0.0f;
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

    /* 아이템 스켈레탈 메쉬 - 장비용 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<USkeletalMesh> ItemSkeletalMesh;

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
		ItemSkeletalMesh = nullptr;
        ItemStat = FBaseStat();
        ItemDamageStat = FBaseDamageStat();
		EquipSocket = NAME_None;
    }

    /* 생성자 */
    FItemBaseInfo(FName InItemID, EItemType InItemType, FString InItemName, FString InItemDescription, bool bInIsStackable, int32 InItemCount, UTexture2D* InItemIcon, UStaticMesh* InItemMesh, USkeletalMesh InItemSkeletalMesh, FBaseStat InItemStat, FBaseDamageStat InItemDamageStat, FName InEquipSocket)
    {
        ItemID = InItemID;
        ItemType = InItemType;
        ItemName = InItemName;
        ItemDescription = InItemDescription;
        bIsStackable = bInIsStackable;
        ItemCount = InItemCount;
        ItemIcon = InItemIcon;
        ItemMesh = InItemMesh;
		ItemSkeletalMesh = InItemSkeletalMesh;
        ItemStat = InItemStat;
        ItemDamageStat = InItemDamageStat;
        EquipSocket = InEquipSocket;
    }

	/* == 연산자 오버로딩 */
	bool operator==(const FItemBaseInfo& Other) const
	{
		return ItemID == Other.ItemID;
	}

	/* != 연산자 오버로딩 */
	bool operator!=(const FItemBaseInfo& Other) const
	{
		return ItemID != Other.ItemID;
	}
};


// 상태 조건 연산용 구조체
USTRUCT(BlueprintType)
struct FConditionInfo
{
	GENERATED_USTRUCT_BODY()

	/* 조건 타입 */
	UPROPERTY(EditAnywhere)
	EConditionType ConditionType;

	/* 조건 연산자 */
	UPROPERTY(EditAnywhere)
	EConditionOperator ConditionOperator;

	/* 조건 값 - Stage 상태에 접근하기 위함 */
    UPROPERTY(EditAnywhere, meta = (EditCondition = "ConditionType==EConditionType::Stage"))
	EStageState StageStateValue;

	/* 조건 값 - Quest 상태에 접근하기 위함 */
    UPROPERTY(EditAnywhere, meta = (EditCondition = "ConditionType==EConditionType::Quest"))
	EQuestState QuestStateValue;

	/* 조건 값 - Stat 상태에 접근하기 위함 */
    UPROPERTY(EditAnywhere, meta = (EditCondition = "ConditionType==EConditionType::Stat"))
	EStatState StatTypeValue;

    /* 조건 값 - Quest, Item에서 사용할 ID */
    UPROPERTY(EditAnywhere, meta = (EditCondition = "ConditionType==EConditionType::Quest || ConditionType==EConditionType::Item"))
	FName PrimaryID;

	/* 조건 값 - Item 수량, Stat 값 */
    UPROPERTY(EditAnywhere, meta = (EditCondition = "ConditionType==EConditionType::Stat || ConditionType==EConditionType::Item"))
	float NumericValue;


    /* 기본 생성자 */
    FConditionInfo()
    {
		ConditionType = EConditionType::None;
		ConditionOperator = EConditionOperator::None;
		StageStateValue = EStageState::None;
		QuestStateValue = EQuestState::None;
		StatTypeValue = EStatState::None;
		PrimaryID = NAME_None;
		NumericValue = 0.0f;
    }

	/* 생성자 */
	FConditionInfo(EConditionType InConditionType, EConditionOperator InConditionOperator, EStageState InStageStateValue, EQuestState InQuestStateValue, EStatState InStatTypeValue, FName InPrimaryID, float InNumericValue)
	{
		ConditionType = InConditionType;
		ConditionOperator = InConditionOperator;
		StageStateValue = InStageStateValue;
		QuestStateValue = InQuestStateValue;
		StatTypeValue = InStatTypeValue;
		PrimaryID = InPrimaryID;
		NumericValue = InNumericValue;
	}
};


// NPC가 대화 시작을 위해 사용하는 대화 정보 구조체
USTRUCT(BlueprintType)
struct FDialogueEntry
{
    GENERATED_USTRUCT_BODY()

    /* NPC ID - 발화자 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName NPCID;

	/* 대화 ID - 발화 내용 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName DialogueID;

    /* 대화 시작 조건 목록 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FConditionInfo> Conditions;

	/* 우선 순위 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int32 Priority;

    /* 기본 생성자 */
	FDialogueEntry()
	{
		NPCID = NAME_None;
		DialogueID = NAME_None;
		Priority = 0;
	}

	/* 생성자 */
	FDialogueEntry(FName InNPCID, FName InDialogueID, TArray<FConditionInfo> InConditions, int32 InPriority)
	{
		NPCID = InNPCID;
		DialogueID = InDialogueID;
		Conditions = InConditions;
		Priority = InPriority;
	}
};


// 대화의 선택지와 다음 대화 ID를 담는 구조체
USTRUCT(BlueprintType)
struct FDialogueOption
{
	GENERATED_USTRUCT_BODY()

	/* 선택지 내용 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText OptionText;

	/* 다음 대화 ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName NextDialogueID;

	/* 기본 생성자 */
	FDialogueOption()
	{
		OptionText = FText::FromString(TEXT("Select Option"));
		NextDialogueID = NAME_None;
	}

	/* 생성자 */
	FDialogueOption(FText InOptionText, FName InNextDialogueID)
	{
		OptionText = InOptionText;
		NextDialogueID = InNextDialogueID;
	}
};



// 대화에 필요한 정보를 담은 대화 구조체
USTRUCT(BlueprintType)
struct FDialogueInfo
{
	GENERATED_USTRUCT_BODY()

	/* 대화 ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName DialogueID;
    
    /* 발화자 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText SpeakerName;

	/* 대화 내용 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText DialogueText;

	/* 대화 선택지 목록 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FDialogueOption> DialogueOptions;

    /* 기본 생성자 */
	FDialogueInfo()
	{
		DialogueID = NAME_None;
		SpeakerName = FText::FromString(TEXT("Speaker"));
		DialogueText = FText::FromString(TEXT("Dialogue Text"));
		DialogueOptions.Add(FDialogueOption());
	}

	/* 생성자 */
	FDialogueInfo(FName InDialogueID, FText InSpeakerName, FText InDialogueText, TArray<FDialogueOption> InDialogueOptions)
	{
		DialogueID = InDialogueID;
		SpeakerName = InSpeakerName;
		DialogueText = InDialogueText;
		DialogueOptions = InDialogueOptions;
	}

};

class IVAN_API IVGenericStructs
{
};
