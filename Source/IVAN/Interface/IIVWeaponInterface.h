// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IVAN/IVGenericStructs.h"
#include "IIVWeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 무기 클래스가 구현하여 AttackComponent와 정보를 주고받기 위한 인터페이스.
 * AttackComponent에 무기 인스턴스가 존재하기 때문에 인터페이스를 거치지 않을 수 있지만,
 * 전달이 필요한 기능들을 정의하고자 사용한다. 
 */

class IVAN_API IIIVWeaponInterface
{
	GENERATED_BODY()

public:
	/* 무기 소유자 전달용 */
	virtual void SetOwnerController(AController* NewOwnerController) = 0;
	virtual void SetOwnerDamageStat(FBaseDamageStat NewDamageStat) = 0;

	virtual int32 GetMaxComboCount() const = 0;

	/* 이번 콤보 카운트에 사용할 몽타주 획득용 */
	virtual UAnimMontage* GetComboMontage(int32 ComboIndex) const = 0;
};
