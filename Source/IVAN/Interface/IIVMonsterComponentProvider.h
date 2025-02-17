// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IIVMonsterComponentProvider.generated.h"

class UIVMonsterStatComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVMonsterComponentProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 몬스터의 스탯 컴포넌트에 접근하기 위한 인터페이스
 */
class IVAN_API IIIVMonsterComponentProvider
{
	GENERATED_BODY()

public:
	virtual UIVMonsterStatComponent* GetMonsterStatComponent() const = 0;
};
