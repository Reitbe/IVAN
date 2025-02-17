// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IIVHitReactionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVHitReactionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * HitReactionComponet와 함께 사용된다.
 * 피격 시 컴포넌트 오너의 행동을 제어하기 위한 인터페이스.
 */
class IVAN_API IIIVHitReactionInterface
{
	GENERATED_BODY()

public:
	/* 피격 리액션 시작 - 캐릭터 상태 전환 등에 활용*/
	virtual void StartHitReaction() = 0;

	/* 피격 리액션 종료 - 캐릭터 상태 전환 등에 활용*/ 
	virtual void EndHitReaction() = 0;

	/* 사망 리액션 종료 - 애니메이션 종료 이후 래그돌 전환 등에 활용*/
	virtual void EndDeathReaction() = 0;
};
