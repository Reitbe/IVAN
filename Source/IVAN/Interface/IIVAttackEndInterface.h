// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IIVAttackEndInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVAttackEndInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * IVAttackEndNotify가 실행되었을 때 호출되는 함수를 정의하는 인터페이스
 * 공격 몽타주의 재생이 종료되었음을 알린다.
 */
class IVAN_API IIIVAttackEndInterface
{
	GENERATED_BODY()

public:
	virtual void AttackEnd() = 0;
};
