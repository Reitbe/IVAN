// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IVAN/IVGenericStructs.h"
#include "IVAttackComponent.generated.h"

class AIVWeapon;

/*
* 공격과 관련된 전반적인 기능을 담당하는 컴포넌트. 공격 몽타주 재생, 콤보 관리 등을 수행한다. 
* Weapon클래스와는 WeaponInstance를 통해 연결되도록 설계했으나, 상위 클래스간 연결로 변경되었다.
* 인터페이스는 그대로 유지하여 필구 구현 함수를 정의하고 있다.
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IVAN_API UIVAttackComponent : public UActorComponent
{
	GENERATED_BODY()

// 기본
public:	
	UIVAttackComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


// 무기
public:
	/* 장비중인 무기를 전달받고 컴포넌트 설정  */
	void SetWeapon(TObjectPtr<AIVWeapon> NewWeapon);

	/* 장비중인 무기를 반환한다 */
	TObjectPtr<AIVWeapon> GetWeapon() const { return WeaponInstance; };

protected:
	/* 현재 장비중인 무기 */
	TObjectPtr<AIVWeapon> WeaponInstance;
	


// 애니메이션
public:
	/* 오너의 애님 인스턴스로 컴포넌트의 애님 인스턴스 설정 */
	void SetAnimInstance(TObjectPtr<UAnimInstance> NewAnimInstance) { AnimInstance = NewAnimInstance; };

protected:
	/* 공격 몽타주 재생을 위한 애님 인스턴스*/
	TObjectPtr<UAnimInstance> AnimInstance;


// 공격
public:
	/* 오너로부터 공격 명령을 전달받아서 수행한다 */
	void Attack(FBaseDamageStat DamageStat);

	/* 공격 종료 후 작업*/
	void AttackEnd();


// 콤보
protected:
	/* 최대 콤보 카운트 - 무기로부터 지정 */
	int32 MaxComboCount;

	/* 현재 콤보 카운트 */
	int32 ComboCount;

	/* 콤보 리셋 시간 */
	float ComboResetTime;

	/* 콤보 리셋 타이머 */
	FTimerHandle ComboTimer;

	/* 콤보 타이머 콜백 */
	void ResetCombo();

};
