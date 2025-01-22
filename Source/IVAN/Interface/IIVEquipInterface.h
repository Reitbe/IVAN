// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IIVEquipInterface.generated.h"

class AIVItemBase;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVEquipInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * EquipComponent와 함께 사용되며 해당 컴포넌트 오너에게 장비를 장착하는 인터페이스
 */
class IVAN_API IIIVEquipInterface
{
	GENERATED_BODY()

public:
	/* 방어구의 경우, 액터를 생성하지 않고 내부 정보와 스켈레탈 메시만 접근하여 장착한다 */
	virtual void EquipByClass(TSubclassOf<AIVItemBase> Item) const = 0;

	/* 무기의 경우, 액터를 생성하여 장착한다 */
	virtual void EquipByInstance(TObjectPtr<AIVItemBase> Item) const = 0;
};
