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
	/* 공격 요청 -> 공격 실행 -> 공격 종료 순서에서 공격이 종료되었을 때 호출되는 함수 */
	virtual void AttackEnd(bool bIsFirstCheck) = 0;

	/* 공격 요청 -> 공격 불가! 요청 거절 시 호출되는 함수 */
	virtual void AttackCancel() = 0;

	virtual void ResetComboEnd() = 0;
};
