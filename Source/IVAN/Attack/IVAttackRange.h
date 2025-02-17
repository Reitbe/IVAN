// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "IVAttackRange.generated.h"

/**
 * 무기, 혹은 캐릭터에서 공격이 진행될 때, 그 공격이 적용되는 범위를 나타내는 캡슐 컴포넌트
 * 직접 충돌하는 것이 아니라 위치와 모양을 전달하는 것이 목적이다.
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class IVAN_API UIVAttackRange : public UCapsuleComponent
{
	GENERATED_BODY()

public:
	UIVAttackRange(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
