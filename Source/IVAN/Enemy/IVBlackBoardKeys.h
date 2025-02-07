// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define BBKEY_TARGET_ACTOR TEXT("TargetActor")
#define BBKEY_TARGET_LOCATION TEXT("TargetLocation")
#define BBKEY_MONSTER_STATE TEXT("MonsterState")
#define BBKEY_DISTANCE_TO_TARGET TEXT("DistanceToTarget")
#define BBKEY_SHORT_ATTACK_DISTANCE TEXT("ShortAttackDistance")
#define BBKEY_LONG_ATTACK_DISTANCE TEXT("LongAttackDistance")

/**
 * 블랙보드와 연결된 키들을 정의하는 클래스
 */


// AI의 상태
UENUM(BlueprintType)
enum class EMonsterState : uint8
{
    Idle            UMETA(DisplayName = "Idle"),
    Patrol          UMETA(DisplayName = "Patrol"),
    Chase           UMETA(DisplayName = "Chase"),
	HitStunned	    UMETA(DisplayName = "HitStunned"),
    BasicAttack     UMETA(DisplayName = "Basic Attack"),
    SpecialAttack   UMETA(DisplayName = "Special Attack"),
    SpecialMove     UMETA(DisplayName = "Special Move"),
    Dead            UMETA(DisplayName = "Dead")
};


class IVAN_API IVBlackBoardKeys
{
};
