// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IIVCharacterComponentProvider.generated.h"

class UCharacterMovementComponent;
class UCharacterTrajectoryComponent;
class UIVCharacterStatComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVCharacterComponentProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 캐릭터 움직임 정보 전달을 위한 인터페이스.
 * 스탯 컴포넌트에 동작 정보가 있기에 이 또한 포함되어있다.
 */

class IVAN_API IIIVCharacterComponentProvider
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UCharacterMovementComponent* GetCharacterMovementComponent() const = 0;
	virtual UCharacterTrajectoryComponent* GetCharacterTrajectoryComponent() const = 0;
	virtual UIVCharacterStatComponent* GetCharacterStatComponent() const = 0;
};
