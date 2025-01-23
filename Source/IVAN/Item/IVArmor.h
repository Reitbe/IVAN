// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Item/IVItemBase.h"
#include "IVArmor.generated.h"

/**
 * 아이템을 상속한 방어구 클래스
 */
UCLASS()
class IVAN_API AIVArmor : public AIVItemBase
{
	GENERATED_BODY()
	
public:
	AIVArmor();

};
