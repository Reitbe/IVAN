// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 캐릭터의 동작 상태를 나타내는 열거형 모음.
 */

// 캐릭터 이동 상태
UENUM(BlueprintType)
enum class EMovementState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
	Move		UMETA(DisplayName = "Move"),
};

// Gait는 걸음걸이를 의미함
UENUM(BlueprintType)
enum class EGaitState : uint8 
{
    Walk        UMETA(DisplayName = "Walk"),
    Run         UMETA(DisplayName = "Run")
};

// 캐릭터 점프 상태
UENUM(BlueprintType)
enum class EJumpState : uint8
{
    InAir       UMETA(DisplayName = "In Air"),
    OnGround    UMETA(DisplayName = "On Ground")
};

// 캐릭터 타겟팅 상태
UENUM(BlueprintType)
enum class ETargetingState : uint8
{
    OnTargeting     UMETA(DisplayName = "On Targeting"),
    NonTargeting    UMETA(DisplayName = "Non-Targeting")
};

// 특수 움직임 상태
UENUM(BlueprintType)
enum class ESpecialMovementState : uint8
{
	None		UMETA(DisplayName = "None"),
    Rolling     UMETA(DisplayName = "Rolling"),
    Dodging     UMETA(DisplayName = "Dodging")
};

class IVAN_API IVCharacterStateEnums
{
};
