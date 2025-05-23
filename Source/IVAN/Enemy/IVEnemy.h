// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Character/IVCharacterBase.h"
#include "IVAN/Enemy/IVBlackBoardKeys.h"
#include "IVAN/Interface/IIVAttackEndInterface.h"
#include "IVAN/Interface/IIVWeaponProvider.h"
#include "IVAN/Interface/IIVEquipInterface.h"
#include "IVAN/Interface/IIVHitReactionInterface.h"
#include "IVAN/Interface/IIVAICharacterBasicCombat.h"
#include "IVAN/Interface/IIVAIControllerBasicCombat.h"
#include "IVAN/Interface/IIVMonsterComponentProvider.h"
#include "IVAN/Interface/IIVLockOnTargetMarker.h"
#include "IVAN/Interface/IIVAIUIControl.h"
#include "IVAN/IVGenericStructs.h"
#include "IVEnemy.generated.h"

class UIVAttackRange;
class UIVHitReactionComponent;
class UIVAttackComponent;
class UIVMonsterStatComponent;
class UWidgetComponent;
class UIVBaseStatBar;

/**
 * 보스몬스터, 일반 몬스터 등의 기본 클래스.
 */
UCLASS()
class IVAN_API AIVEnemy 
	: public AIVCharacterBase
	, public IIIVWeaponProvider
	, public IIIVAttackEndInterface
	, public IIIVEquipInterface
	, public IIIVHitReactionInterface
	, public IIIVAICharacterBasicCombat
	, public IIIVMonsterComponentProvider
	, public IIIVLockOnTargetMarker
	, public IIIVAIUIControl
{
	GENERATED_BODY()

// 기본 요소
public:
	AIVEnemy();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	FName MonsterName;

	/* 보스 몬스터와 일반 몬스터를 구분하기 위한 타입*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	EMonsterType MonsterType;


// 스탯 시스템
public:
	/* 정말 기본적인 스탯 제공 함수 */
	FName GetMonsterName() const { return MonsterName; }
	FBaseStat GetMonsterBaseStat() const;

protected:
	/* 몬스터 스탯 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TObjectPtr<UIVMonsterStatComponent> MonsterStatComponent;


// 장비 관련
	/* 몬스터에 부착된 공격 컴포넌트 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	TArray<UIVAttackRange*> AttackRanges;


// 데미지 처리
public:
	/* 공격 종합 처리 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UIVAttackComponent> AttackComponent;

	/* 피격 리액션 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction")
	TObjectPtr<UIVHitReactionComponent> HitReactionComponent;

protected:
	/* 사망 및 부활*/
	virtual void SetDead() override;
	virtual void SetAlive() override;

	/* 피격 정보 수신 */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


// 애니메이션
 	/* 몽타주 재생 전후 대리자 연결을 위한 애님 인스턴스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TObjectPtr<UAnimInstance> AnimInstance;
 
 
// 위젯
public:
	/* 체력 위젯 장착용 위젯 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UWidgetComponent> WidgetComponent;

	/* 체력 바 위젯 */
	TObjectPtr<UIVBaseStatBar> HealthBar;


// 인터페이스 구현
public:
	/* IIIVWeaponProvider 인터페이스->WeaponHitDetection에서 캐릭터가 장착중인 무기 접근용 */
	virtual TObjectPtr<AIVWeapon> GetWeapon() const override;

	/* IIIVAttackEndInterface 인터페이스->몽타주의 공격 종료 시점 전달용 */
	virtual void AttackEnd(bool bIsFirstCheck) override;
	virtual void AttackCancel() override;
	virtual void ResetComboEnd() override;

	/* IIIVEquipInterface 인터페이스->장비 장착용 */
	virtual UIVEquipComponent* GetEquipComponent() const override { return EquipComponent; }
	virtual void EquipByInstance(TObjectPtr<AIVWeapon> Weapon, FName EquipSocket) const override;
	virtual TArray<USkeletalMeshComponent*>& GetEquipMeshArray() override { return EquipMeshes; }
	virtual void UnEquipWeapon() override;

	/* IIIVHitReactionInterface 인터페이스->피격 리액션용 */
	virtual void StartHitReaction() override;
	virtual void EndHitReaction() override;
	virtual void EndDeathReaction() override;

	/* IIIVAICharacterBasicCombat 인터페이스->기본 공격용 */
	virtual void ExecuteBasicAttack() override;

	/* IIIVMonsterComponentProvider 인터페이스->몬스터 스탯 컴포넌트 접근용 */
	virtual UIVMonsterStatComponent* GetMonsterStatComponent() const override { return MonsterStatComponent; };

	/* IIIVAIUIControl 인터페이스 -> 위젯 제어용 */
	virtual void ShowStatWidget() const override;
	virtual void HideStatWidget() const override;

};
