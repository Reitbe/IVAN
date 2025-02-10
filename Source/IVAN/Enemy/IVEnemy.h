// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Character/IVCharacterBase.h"
#include "IVAN/Interface/IIVAttackEndInterface.h"
#include "IVAN/Interface/IIVWeaponProvider.h"
#include "IVAN/Interface/IIVEquipInterface.h"
#include "IVAN/Interface/IIVHitReactionInterface.h"
#include "IVAN/Interface/IIVAICharacterBasicCombat.h"
#include "IVAN/Interface/IIVAIControllerBasicCombat.h"
#include "IVAN/Interface/IIVMonsterComponentProvider.h"
#include "IVEnemy.generated.h"

class UIVHitReactionComponent;
class UIVAttackComponent;
class UIVMonsterStatComponent;
class UWidgetComponent;
class UIVBaseStatBar;

/**
 * AI 적 캐릭터 클래스
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
{
	GENERATED_BODY()

// 기본 요소
public:
	AIVEnemy();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


// 스탯 시스템
protected:
	/* 몬스터 스탯 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TObjectPtr<UIVMonsterStatComponent> MonsterStatComponent;


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

	/* IIIVEquipInterface 인터페이스->장비 장착용 */
	virtual void EquipByClass(TSubclassOf<AIVItemBase> Item) const override;
	virtual void EquipByInstance(TObjectPtr<AIVItemBase> Item) const override;

	/* IIIVHitReactionInterface 인터페이스->피격 리액션용 */
	virtual void StartHitReaction() override;
	virtual void EndHitReaction() override;
	virtual void EndDeathReaction() override;

	/* IIIVAICharacterBasicCombat 인터페이스->기본 공격용 */
	virtual void ExecuteBasicAttack() override;

	/* IIIVMonsterComponentProvider 인터페이스->몬스터 스탯 컴포넌트 접근용 */
	virtual UIVMonsterStatComponent* GetMonsterStatComponent() const override { return MonsterStatComponent; };
};
