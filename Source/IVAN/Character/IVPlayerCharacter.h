// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Character/IVCharacterBase.h"
#include "IVAN/Interface/IIVCharacterComponentProvider.h"
#include "InputActionValue.h"
#include "IVPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UInputComponent;
class UAnimInstance;
class USpringArmComponent;
class UCameraComponent;
class UCharacterTrajectoryComponent;

/**
 * 인게임에 사용되는 플레이어 캐릭터.
 */

UCLASS()
class IVAN_API AIVPlayerCharacter : public AIVCharacterBase, public IIIVCharacterComponentProvider
{
	GENERATED_BODY()

// 기본 요소
public:
	AIVPlayerCharacter();

public:
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

protected:
	/* 캐릭터를 조종하는 플레이어 컨트롤러*/
	TObjectPtr<APlayerController> PlayerController;

// 모션 매칭
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Matching")
	TObjectPtr<UCharacterTrajectoryComponent> TrajectoryComponent;

	/* 애님 인스턴스에 정보 제공을 위한 IIIVCharacterComponentProvider 인터페이스 구현*/
	virtual UCharacterTrajectoryComponent* GetCharacterTrajectoryComponent() const override { return TrajectoryComponent; }
	virtual UCharacterMovementComponent* GetCharacterMovementComponent() const override { return GetCharacterMovement(); }
	virtual UIVCharacterStatComponent* GetCharacterStatComponent() const override { return CharacterStatComponent; }


// 구르기, 닷지와 같은 특수 움직임용 몽타주 로드 헬퍼
protected:
	void MontageConstructHelper();

protected:
	/* 몽타주 재생 전후 대리자 연결을 위한 애님 인스턴스 - 생성자 로드 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TObjectPtr<UAnimInstance> AnimInstance;

	TSoftObjectPtr<UAnimMontage> RollMontage;
	TSoftObjectPtr<UAnimMontage> DodgeRightMontage;
	TSoftObjectPtr<UAnimMontage> DodgeLeftMontage;
	TSoftObjectPtr<UAnimMontage> DodgeBackMontage;

// 공격 처리
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


// 입력
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	/* 2D평면 움직임 및 시선 이동 */
	void BasicMove(const FInputActionValue& Value);
	void SpecialMove(const FInputActionValue& Value); // 구르기, 닷지
	void Look(const FInputActionValue& Value);
	
	/* 상태 전환 */
	void RunWalkSwitch(const FInputActionValue& Value); // 걷기 달리기 전환

	/* 공격 */
	void BasicAttack(const FInputActionValue& Value); // 기본 공격


private:
	void InputConstructHelper();

protected:
	TObjectPtr<UInputMappingContext> InputMappingContext;

	/* 일반 공격과 특수 공격*/
	TObjectPtr<UInputAction> BasicAttackAction;
	TObjectPtr<UInputAction> SpecialAttack;

	/* 평면 이동, 점프, 시선 처리와 같은 기본 움직임 */
	TObjectPtr<UInputAction> BasicMovement; // 2D 평면상의 움직임
	TObjectPtr<UInputAction> LookAction; // 시선
	TObjectPtr<UInputAction> JumpAction; // 점프

	/* 특수 상호작용*/
	TObjectPtr<UInputAction> TargetAction;
	TObjectPtr<UInputAction> InteractAction;
	TObjectPtr<UInputAction> DefendAction;
	TObjectPtr<UInputAction> SpecialMovement; // 구르기, 닷지
	TObjectPtr<UInputAction> RunWalkSwitchAction; // 걷기 & 달리기 전환

	/* 아이템 사용*/
	TObjectPtr<UInputAction> UseFirstItemSlot;
	TObjectPtr<UInputAction> UseSecondItemSlot;
	TObjectPtr<UInputAction> UseThirdItemSlot;
	TObjectPtr<UInputAction> UseFourthItemSlot;


// 카메라
protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;
};
