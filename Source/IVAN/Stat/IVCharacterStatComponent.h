// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IVAN/IVGenericStructs.h"
#include "IVAN/Enums/IVCharacterStateEnums.h"
#include "IVCharacterStatComponent.generated.h"

DECLARE_DELEGATE_OneParam(FBaseStatChangedDelegate, const FBaseStat&);

class UCharacterMovementComponent;

/*
* 캐릭터 정보를 관리하는 컴포넌트.
* 움직임 정보, 체력(미구현) 등을 관리한다.
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IVAN_API UIVCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

// 기본
public:	
	UIVCharacterStatComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// 데미지 처리
public:
	/* 캐릭터의 TakeDamage 함수를 그대로 넘겨받아 데미지 처리*/
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);


// 캐릭터 기본 스탯
public:
	/* 스탯 변경 시 정보 전달 담당 대리자 */
	FBaseStatChangedDelegate OnBaseStatChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats");
	FBaseStat BaseStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats");
	FBaseDamageStat BaseDamageStat;

public:
	/* 스탯 간 더하기 연산 -> 아이템 사용, 장비 장착 등*/
	void AttachStat(const FBaseStat& OtherStat); // 위젯 갱신을 위한 대리자 호출
	void AttachStat(const FBaseDamageStat& OtherStat);

	/* 스탯 간 빼기 연산 -> 장비 해제 등 */ 
	void DetachStat(const FBaseStat& OtherStat); // 위젯 갱신을 위한 대리자 호출
	void DetachStat(const FBaseDamageStat& OtherStat);

	/* 스텟을 불러와 적용하는 용도 */
	void SetBaseStat(const FBaseStat& NewBaseStat); // 위젯 갱신을 위한 대리자 호출
	void SetBaseDamageStat(const FBaseDamageStat& NewBaseDamageStat);

	FBaseStat GetBaseStat() const { return BaseStat; };


// 캐릭터 동작 상태 정보
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State");
	EMovementState MovementState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State");
	EGaitState GaitState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State");
	EJumpState JumpState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State");
	ETargetingState TargetingState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State");
	ESpecialMovementState SpecialMovementState;
	
	/* 점프와 움직임 정지 여부를 확인하기 위한 캐릭터 무브먼트 컴포넌트 */
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

// 캐릭터 상태 변경 및 획득
public:
	void SetCharacterMovementState(EMovementState NewState) { MovementState = NewState; };
	void SetCharacterGaitState(EGaitState NewState) { GaitState = NewState; };
	void SetCharacterJumpState(EJumpState NewState) { JumpState = NewState; };
	void SetCharacterTargetingState(ETargetingState NewState) { TargetingState = NewState; };
	void SetCharacterSpecialMoveState(ESpecialMovementState NewState) { SpecialMovementState = NewState; };

public:
	virtual EMovementState GetCharacterMovementState() const { return MovementState; };
	virtual EGaitState GetCharacterGaitState() const { return GaitState; };
	virtual EJumpState GetCharacterJumpState() const { return JumpState; };
	virtual ETargetingState GetCharacterTargetingState() const { return TargetingState; };
	virtual ESpecialMovementState GetCharacterSpecialMovementState() const { return SpecialMovementState; };
};
