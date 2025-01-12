// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * ĳ������ ���� ���¸� ��Ÿ���� ������ ����.
 */

// ĳ���� �̵� ����
UENUM(BlueprintType)
enum class EMovementState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
	Move		UMETA(DisplayName = "Move"),
};

// Gait�� �������̸� �ǹ���
UENUM(BlueprintType)
enum class EGaitState : uint8 
{
    Walk        UMETA(DisplayName = "Walk"),
    Run         UMETA(DisplayName = "Run")
};

// ĳ���� ���� ����
UENUM(BlueprintType)
enum class EJumpState : uint8
{
    InAir       UMETA(DisplayName = "In Air"),
    OnGround    UMETA(DisplayName = "On Ground")
};

// ĳ���� Ÿ���� ����
UENUM(BlueprintType)
enum class ETargetingState : uint8
{
    OnTargeting     UMETA(DisplayName = "On Targeting"),
    NonTargeting    UMETA(DisplayName = "Non-Targeting")
};

// Ư�� ������ ����
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
