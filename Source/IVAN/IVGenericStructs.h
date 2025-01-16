// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVGenericStructs.generated.h"

/**
 * 게임에 사용되는 구조체들을 모아놓은 클래스.
 */

// 캐릭터, 아이템이 공동으로 사용하는 기본 스탯 구조체
USTRUCT(BlueprintType) 
struct FBaseStat
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float CurrentHP;

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

	FBaseStat(float InMaxHP, float InCurrentHP, float InMaxStamina, float InCurrentStamina, float InBaseDamage, float InAdditionalDamage, float InDamageReduction)
	{
		MaxHP = InMaxHP;
		CurrentHP = InCurrentHP;
		MaxStamina = InMaxStamina;
		CurrentStamina = InCurrentStamina;
	}

    // 연산자 오버로딩은 멤버 초기화를 유지하도록 작성
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

USTRUCT(BlueprintType)
struct FBaseDamageStat
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float BaseDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float AdditionalDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float DamageReduction;

    FBaseDamageStat()
    {
        BaseDamage = 10.0f;
        AdditionalDamage = 0.0f;
        DamageReduction = 0.0f;
    }

    FBaseDamageStat(float InMaxHP, float InCurrentHP, float InMaxStamina, float InCurrentStamina, float InBaseDamage, float InAdditionalDamage, float InDamageReduction)
    {
        BaseDamage = InBaseDamage;
        AdditionalDamage = InAdditionalDamage;
        DamageReduction = InDamageReduction;
    }

    // 연산자 오버로딩은 멤버 초기화를 유지하도록 작성
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
