// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IVAN/Enums/IVCharacterStateEnums.h"
#include "IIVCharacterStateProvider.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVCharacterStateProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 애님 인스턴스와 스탯 컴포넌트간 연결을 위하여 사용하려 했으나, 애님 인스턴스에서 스탯 컴포넌트를 직접 참조하는 방식으로 변경함.
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
