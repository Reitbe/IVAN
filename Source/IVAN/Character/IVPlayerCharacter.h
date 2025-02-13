// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Character/IVCharacterBase.h"
#include "IVAN/Interface/IIVCharacterComponentProvider.h"
#include "IVAN/Interface/IIVAttackEndInterface.h"
#include "IVAN/Interface/IIVWeaponProvider.h"
#include "IVAN/Interface/IIVEquipInterface.h"
#include "IVAN/Interface/IIVHitReactionInterface.h"
#include "IVAN/Interface/IIVLockOnTargetMarker.h"
#include "InputActionValue.h"
#include "IVPlayerCharacter.generated.h"

class UIVAttackRange;
class UInputMappingContext;
class UInputAction;
class UInputComponent;
class UAnimInstance;
class USpringArmComponent;
class UCameraComponent;
class UCharacterTrajectoryComponent;
class UIVCharacterStatComponent;
class UIVHitReactionComponent;
class UIVAttackComponent;

/**
 * 인게임 플레이어 캐릭터 클래스
 * 여러 컴포넌트를 보유하고 있으며, 주로 플레이어의 입력에 따른 동작을 수행한다.
 */

UCLASS()
class IVAN_API AIVPlayerCharacter
	: public AIVCharacterBase
	,public IIIVCharacterComponentProvider
	,public IIIVWeaponProvider
	,public IIIVAttackEndInterface
	,public IIIVEquipInterface
	,public IIIVHitReactionInterface
{
	GENERATED_BODY()

// 기본 요소
public:
	AIVPlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

private:
	/* 캐릭터 플레이어 컨트롤러*/
	TObjectPtr<APlayerController> PlayerController;


// 스텟 시스템
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TObjectPtr<UIVCharacterStatComponent> CharacterStatComponent;


// 장비 관련
public:
	/* 공격 컴포넌트에 무기 인스턴스 전달 */
	void SetWeaponOnWeaponComponent(TObjectPtr<AIVWeapon> Weapon);

	/* 캐릭터에 부착된 공격 컴포넌트 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	TArray<UIVAttackRange*> AttackRanges;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	//TObjectPtr<UIVAttackRange> RightCalfAttackRange;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	//TObjectPtr<UIVAttackRange> LeftCalfAttackRange;


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

	/* 새로운 캐릭터 생성(부활) 및 빙의 완료시 실행*/
	void RespawnComplete();

	/* 피격 정보 수신 */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


// 락온(타겟팅)
protected:
	void LockOn();	// 락온
	void LockOff(); // 락오프
	void CheckLockOnDistance(); // 락온 거리 체크
	void MonsterDeath(AActor* DeadMonster); // 타겟이 죽었을 때 처리

	/* 현재 타겟팅중인 대상 */
	TObjectPtr<AActor> LockOnActor; 

	/* 타겟팅이 가능한 & 타겟팅이 유지되는 거리 */
	float LockOnDistance; 

	/* 일정 시간마다 대상이 락온 범위 내에 있는지 확인하기 위한 타이머 */
	FTimerHandle LockOnCheckTimer; 


// 입력
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	/* 기본 움직임 */
	void BasicMove(const FInputActionValue& Value);		// 2D 평면 이동
	void SpecialMove(const FInputActionValue& Value);   // 구르기, 닷지
	void Look(const FInputActionValue& Value);			// 시선 이동
	
	/* 상태 전환 */
	void RunWalkSwitch(); // 걷기 달리기 전환

	/* 공격 */
	void BasicAttack(const FInputActionValue& Value);	// 기본 공격
	uint8 bAttackEndChecked : 1;						// 공격 종료 체크는 1~2번 처리된다. 회차 표시용.

	/* 락온 */
	void LockOnSwitch(); // 락온 전환
	
private:
	/* 입력 관련 에셋들 로드 및 초기화 헬퍼 */
	void InputConstructHelper();

protected:
	TObjectPtr<UInputMappingContext> InputMappingContext;

	/* 일반 공격과 특수 공격*/
	TObjectPtr<UInputAction> BasicAttackAction;
	TObjectPtr<UInputAction> SpecialAttack;

	/* 평면 이동, 점프, 시선 처리와 같은 기본 움직임 */
	TObjectPtr<UInputAction> BasicMovement; 
	TObjectPtr<UInputAction> LookAction; 
	TObjectPtr<UInputAction> JumpAction; 

	/* 특수 상호작용*/
	TObjectPtr<UInputAction> TargetAction;
	TObjectPtr<UInputAction> InteractAction;
	TObjectPtr<UInputAction> DefendAction;
	TObjectPtr<UInputAction> SpecialMovement; 
	TObjectPtr<UInputAction> RunWalkSwitchAction; 

	/* 아이템 사용 - 현재 미구현*/
	TObjectPtr<UInputAction> UseFirstItemSlot;
	TObjectPtr<UInputAction> UseSecondItemSlot;
	TObjectPtr<UInputAction> UseThirdItemSlot;
	TObjectPtr<UInputAction> UseFourthItemSlot;

	
// 애니메이션
public:
	/* 모션 매칭에 사용하는 움직임 추적 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Matching")
	TObjectPtr<UCharacterTrajectoryComponent> TrajectoryComponent;

	/* 몽타주 재생 전후 대리자 연결을 위한 애님 인스턴스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TObjectPtr<UAnimInstance> AnimInstance;

private:
	/* 몽타주 에셋 로드를 위한 헬퍼 */
	void MontageConstructHelper();

	/* 특수 움직임에 사용되는 몽타주들 */
	TSoftObjectPtr<UAnimMontage> RollMontage;
	TSoftObjectPtr<UAnimMontage> DodgeRightMontage;
	TSoftObjectPtr<UAnimMontage> DodgeLeftMontage;
	TSoftObjectPtr<UAnimMontage> DodgeBackMontage;


// 카메라
protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;


// 인터페이스 구현
public:
	/* IIIVCharacterComponentProvider 인터페이스->움직임 및 상태 정보 제공용 */
	virtual UCharacterTrajectoryComponent* GetCharacterTrajectoryComponent() const override { return TrajectoryComponent; }
	virtual UCharacterMovementComponent* GetCharacterMovementComponent() const override { return GetCharacterMovement(); }
	virtual UIVCharacterStatComponent* GetCharacterStatComponent() const override { return CharacterStatComponent; }

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
};
