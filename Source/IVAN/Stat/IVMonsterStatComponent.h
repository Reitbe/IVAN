// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Stat/IVBaseStatComponent.h"
#include "IVAN/Enemy/IVBlackBoardKeys.h"
#include "IVMonsterStatComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FHpChangedDelegate, const float /*MaxHp*/, const float /*CurrentHp*/);

class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class IVAN_API UIVMonsterStatComponent : public UIVBaseStatComponent
{
	GENERATED_BODY()
	
// 기본
public:
	UIVMonsterStatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;


// 정보 갱신 관련
public:
	/* 체력 변경 정보를 전달하는 대리자 */
	FHpChangedDelegate OnHpChanged;

protected:
	virtual void SetDead() override;	// 사망 처리
	virtual void SetAlive() override;	// 부활 처리


// 스탯 연산
public:
	/* 스탯 간 더하기 연산 -> 아이템 사용, 장비 장착 등*/
	virtual void AttachStat(const FBaseStat& OtherStat) override;

	/* 스탯 간 빼기 연산 -> 장비 해제 등 */
	virtual void DetachStat(const FBaseStat& OtherStat) override; 

	/* 스텟을 불러와 적용하는 용도 */
	virtual void SetBaseStat(const FBaseStat& NewBaseStat) override;


// 몬스터 동작 상태 정보
	/* 점프와 움직임 정지 여부를 확인하기 위한 캐릭터 무브먼트 컴포넌트 */
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State");
	EMonsterState MonsterState;


// 몬스터 상태 변경 및 획득(블랙보드에서 우선 수정, 이후 이곳 수정)
public:
	void SetMonsterState(EMonsterState NewState) { MonsterState = NewState; };
	EMonsterState GetMonsterState() const { return MonsterState; };


// 데미지 처리
public:
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
