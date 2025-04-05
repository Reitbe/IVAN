// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IVAN/IVGenericStructs.h"
#include "IVEquipComponent.generated.h"

class AIVWeapon;
class AIVArmor;
class USkeletalMeshComponent;

/*
* 플레이어의 장비(무기, 방어구) 관리를 담당하는 컴포넌트
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IVAN_API UIVEquipComponent : public UActorComponent
{
	GENERATED_BODY()

// 기본
public:	
	UIVEquipComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	
// 무기 정보
protected:
	/* 장착중인 무기 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AIVWeapon> WeaponClass;

	/* 장착중인 무기 인스턴스 */
	TObjectPtr<AIVWeapon> WeaponInstance;


// 무기 탈착 관련 함수
public:
	/* 인벤토리의 장비 슬롯에 놓인 무기를 장착한다. */
	UFUNCTION()
	void EquipWeapon();

	/* 캐릭터 장비컴포넌트에 지정된 무기를 장착한다 */
	void EquipDefaultWeapon();

	/* 무기를 레벨에 드롭한다 */
	void DropWeapon();

	/* 무기를 해제하고 맨손으로 돌아간다 */
	void UnEquipWeapon();

	/* 장착중인 무기를 반환한다 */
	TObjectPtr<AIVWeapon> GetWeapon() const { return WeaponInstance; };


// 방어구 탈착 관련 함수
public:
	/* 방어구를 장착한다. */
	UFUNCTION()
	void EquipArmors();
};
