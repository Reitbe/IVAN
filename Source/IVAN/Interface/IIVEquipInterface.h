// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IIVEquipInterface.generated.h"

class AIVWeapon;
class UIVEquipComponent;

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
	/* 컴포넌트 제공자 */
	virtual UIVEquipComponent* GetEquipComponent() const = 0;

	/* 장비 컴포넌트가 무기를 생성하고 캐릭터에 이를 전달한다.  */
	virtual void EquipByInstance(TObjectPtr<AIVWeapon> Weapon, FName EquipSocket) const = 0;

	/* 방어구 장착을 위한 메쉬 목록 레퍼런스 제공 */
	virtual TArray<USkeletalMeshComponent*>& GetEquipMeshArray() = 0;

	/* 장착중인 무기 해제 시 */
	virtual void UnEquipWeapon() = 0;
};
