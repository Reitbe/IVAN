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
 * ĳ���� ������ ���� ������ ���� �������̽�.
 * ���� ������Ʈ�� ���� ������ �ֱ⿡ �� ���� ���ԵǾ��ִ�.
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
