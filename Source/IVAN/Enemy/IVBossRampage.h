// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Enemy/IVBossEnemy.h"
#include "IVBossRampage.generated.h"

class UMotionWarpingComponent;
class UAnimMontage;

/**
 * 보스몬스터 중 램페이지 보스를 위한 클래스.
 * 특수 공격(땅바닥 내려찍기)와 특수 이동(원거리 점프)를 가지며, 관련 데이터 또한 여기서 처리한다.
 */
UCLASS()
class IVAN_API AIVBossRampage : public AIVBossEnemy
{
	GENERATED_BODY()

// 기본
public:
	AIVBossRampage();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


// 모션 워핑
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Motion Warping")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;


// 램페이지 공격 범위 - 양손에 각각 하나씩 장착
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack Range")
	TObjectPtr<UIVAttackRange> RightHandAttackRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack Range")
	TObjectPtr<UIVAttackRange> LeftHandAttackRange;


// 램페이지 특수 동작
public:
	/* 인터페이스 오버라이드 */
	virtual void SpecialMove(AActor* TargetActor) override;
	virtual void SpecialAttack(AActor* TargetActor) override;
	virtual void OnNotifySpecialMoveHit() override;
	virtual void OnNotifySpecialAttackHit() override;

	void OnSpecialMoveEnd(AActor* TargetActor);
	void OnSpecialAttackEnd(AActor* TargetActor);

protected:
	/* 특수 이동 중 점프 준비부분. 캐릭터 방향으로 회전을 담당한다. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpecialBehavior")
	TObjectPtr<UAnimMontage> SpecialMoveMontage_FirstPart;

	/* 특수 이동 중 점프부분. 날아가는 동안 재생되는 애니메이션이다. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpecialBehavior")
	TObjectPtr<UAnimMontage> SpecialMoveMontage_SecondPart;

	/* 땅을 내려찍는 특수 공격 애니메이션*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpecialBehavior")
	TObjectPtr<UAnimMontage> SpecialAttackMontage;

	/* 날아가서 땅을 찍을 때 1번 적용 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpecialBehavior")
	float SpecialMoveDamage;

	/* 땅을 내려찍을 때 오른손 왼손 각각 적용*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpecialBehavior")
	float SpecialAttackDamage;


private:
	/* 특수 이동 시 실제로 캐릭터를 이동시키는 함수 */
	void StartLaunchCharacter(AActor* TargetActor);
};
