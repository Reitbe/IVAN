// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IVAN/IVGenericStructs.h"
#include "IVAN/GameSystem/IVDeathEventSubsystem.h"
#include "IVBaseStatComponent.generated.h"

DECLARE_DELEGATE_OneParam(FBaseStatChangedDelegate, const FBaseStat&);

/*
* 캐릭터, 몬스터의 기본 스탯 정보를 관리하는 컴포넌트.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IVAN_API UIVBaseStatComponent : public UActorComponent
{
	GENERATED_BODY()

// 기본
public:	
	UIVBaseStatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;


// 정보 갱신 관련
public:
	/* 사망, 부활 이벤트를 담당하는 글로벌 이벤트 시스템 */
	TObjectPtr<UIVDeathEventSubsystem> LifeEventSubsystem;

	/* 기본 스탯 변경 정보를 전달하는 대리자 */
	FBaseStatChangedDelegate OnBaseStatChanged;


// 스탯 정보(체력, 스테미나, 데미지)
protected:
	/* 체력, 스테미나가 담긴 기본 스탯 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats");
	FBaseStat BaseStat;

	/* 기본 공격력, 추가 공력력, 방어력이 담긴 데미지 스텟*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats");
	FBaseDamageStat BaseDamageStat;

	virtual void SetDead();	// 사망 처리
	virtual void SetAlive();// 부활 처리

public:
	/* 스탯 간 더하기 연산 -> 아이템 사용, 장비 장착 등*/
	virtual void AttachStat(const FBaseStat& OtherStat); // 위젯 갱신을 위한 대리자 호출
	virtual void AttachStat(const FBaseDamageStat& OtherStat);

	/* 스탯 간 빼기 연산 -> 장비 해제 등 */ 
	virtual void DetachStat(const FBaseStat& OtherStat); // 위젯 갱신을 위한 대리자 호출
	virtual void DetachStat(const FBaseDamageStat& OtherStat);

	/* 스텟을 불러와 적용하는 용도 */
	virtual void SetBaseStat(const FBaseStat& NewBaseStat); // 위젯 갱신을 위한 대리자 호출
	virtual void SetBaseDamageStat(const FBaseDamageStat& NewBaseDamageStat);

	FBaseStat GetBaseStat() const { return BaseStat; };
	FBaseDamageStat GetBaseDamageStat() const { return BaseDamageStat; };


public:
	/* 캐릭터의 TakeDamage 함수를 그대로 넘겨받아 데미지 처리*/
	virtual bool TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

};
