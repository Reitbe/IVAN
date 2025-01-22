// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Item/IVItemBase.h"
#include "IVConsumable.generated.h"

/**
 * 아이템을 상속받은 소비 아이템 클래스
 */
UCLASS()
class IVAN_API AIVConsumable : public AIVItemBase
{
	GENERATED_BODY()
	
public:
	AIVConsumable();


};
