// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IVEquipComponent.generated.h"

class AIVWeapon;
class AIVArmor;

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
	/* 무기를 장착한다. */
	void EquipWeapon();

	/* 무기를 해제한다. */
	void UnequipWeapon();

	/* 무기를 교체한다 */
	void ChangeWeapon(TSubclassOf<AIVWeapon> NewWeaponClass);

	/* 장착중인 무기를 반환한다 */
	TObjectPtr<AIVWeapon> GetWeapon() const { return WeaponInstance; };


// 방어구 탈착 관련 함수
	/* 방어구를 장착한다. */
	void EquipArmor();

	/* 방어구를 해제한다. */
	void UnequipArmor();

	/* 방어구를 교체한다 */
	void ChangeArmor(TSubclassOf<AIVArmor> NewArmorClass);
};
