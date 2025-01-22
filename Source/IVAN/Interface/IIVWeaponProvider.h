// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IIVWeaponProvider.generated.h"

class AIVWeapon;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVWeaponProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 특정 대상 소유의 무기에 접근하기 위한 인터페이스
 * HitDetection AnimNotifyState 에서 오너의 무기를 가져오기 위해 사용된다
 */
class IVAN_API IIIVWeaponProvider
{
	GENERATED_BODY()

public:
	virtual TObjectPtr<AIVWeapon> GetWeapon() const = 0;
};
