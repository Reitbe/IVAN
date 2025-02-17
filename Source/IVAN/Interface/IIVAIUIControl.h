// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IIVAIUIControl.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVAIUIControl : public UInterface
{
	GENERATED_BODY()
};

/**
 * 몬스터 AI의 체력 위젯을 제어하기 위한 인터페이스
 */
class IVAN_API IIIVAIUIControl
{
	GENERATED_BODY()

public:
	virtual void ShowStatWidget() const = 0;
	virtual void HideStatWidget() const = 0;
};
