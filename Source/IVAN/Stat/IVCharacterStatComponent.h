// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Stat/IVBaseStatComponent.h"
#include "Components/ActorComponent.h"
#include "IVAN/Enums/IVCharacterStateEnums.h"
#include "IVCharacterStatComponent.generated.h"

class UCharacterMovementComponent;

/*
* 캐릭터 동작 및 스탯 정보를 관리하는 컴포넌트.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IVAN_API UIVCharacterStatComponent : public UIVBaseStatComponent
{
	GENERATED_BODY()

// 기본
public:	
	UIVCharacterStatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;


// 정보 갱신 관련
public:
	void UseConsumableItem(FBaseStat& ItemBaseStat, FBaseDamageStat& ItemDamageStat);	// 소비 아이템 사용 처리
	void EquipItem(FBaseStat& ItemBaseStat, FBaseDamageStat& ItemDamageStat);			// 장비 아이템 장착 처리
	void UnequipItem(FBaseStat& ItemBaseStat, FBaseDamageStat& ItemDamageStat);			// 장비 아이템 해제 처리

protected:
	virtual void SetDead() override;	// 사망 처리
	virtual void SetAlive() override;	// 부활 처리


// 캐릭터 동작 상태 정보
protected:
	/* 점프와 움직임 정지 여부를 확인하기 위한 캐릭터 무브먼트 컴포넌트 */
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State");
	ELifeState LifeState;

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
	

// 캐릭터 상태 변경 및 획득
public:
	void SetCharacterMovementState(EMovementState NewState) { MovementState = NewState; };
	void SetCharacterGaitState(EGaitState NewState) { GaitState = NewState; };
	void SetCharacterJumpState(EJumpState NewState) { JumpState = NewState; };
	void SetCharacterTargetingState(ETargetingState NewState) { TargetingState = NewState; };
	void SetCharacterSpecialMoveState(ESpecialMovementState NewState) { SpecialMovementState = NewState; };
	void SetCharacterLifeState(ELifeState NewState) { LifeState = NewState; };

	EMovementState GetCharacterMovementState() const { return MovementState; };
	EGaitState GetCharacterGaitState() const { return GaitState; };
	EJumpState GetCharacterJumpState() const { return JumpState; };
	ETargetingState GetCharacterTargetingState() const { return TargetingState; };
	ESpecialMovementState GetCharacterSpecialMovementState() const { return SpecialMovementState; };
	ELifeState GetLifeState() const { return LifeState; };
	

// 데미지 처리
public:
	/* 캐릭터의 TakeDamage 함수를 그대로 넘겨받아 데미지 처리*/
	virtual bool TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};
