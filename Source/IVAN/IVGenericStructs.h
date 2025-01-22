// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

class IVAN_API IVGenericStructs
{
};
