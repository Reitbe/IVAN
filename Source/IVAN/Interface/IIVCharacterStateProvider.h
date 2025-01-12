// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IVAN/IVCharacterStateEnums.h"
#include "IIVCharacterStateProvider.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVCharacterStateProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * �̻�� �������̽�! 
 * �ִ� �ν��Ͻ��� ���� ������Ʈ�� ������ ���Ͽ� ����Ϸ� ������, �ִ� �ν��Ͻ����� ���� ������Ʈ�� ���� �����ϴ� ������� ������.
 */

class IVAN_API IIIVCharacterStateProvider
{
	GENERATED_BODY()

public:
	virtual EMovementState GetCharacterMovementState() const = 0;
	virtual EGaitState GetCharacterGaitState() const = 0;
	virtual EJumpState GetCharacterJumpState() const = 0;
	virtual ETargetingState GetCharacterTargetingState() const = 0;
};
