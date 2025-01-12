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
	TObjectPtr<APlayerController> PlayerController;

// 모션 매칭
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Matching")
	TObjectPtr<UCharacterTrajectoryComponent> TrajectoryComponent;

	/* 애님 인스턴스에 정보 제공을 위한 IIIVCharacterComponentProvider 인터페이스 구현*/
	virtual UCharacterTrajectoryComponent* GetCharacterTrajectoryComponent() const override { return TrajectoryComponent; }
	virtual UCharacterMovementComponent* GetCharacterMovementComponent() const override { return GetCharacterMovement(); }
	virtual UIVCharacterStatComponent* GetCharacterStatComponent() const override { return CharacterStatComponent; }


// 구르기, 닷지와 같은 특수 움직임용 몽타주
protected:
	void MontageConstructHelper();

protected:
	/* 몽타주 재생 전후 대리자 연결을 위한 애님 인스턴스 - 생성자 로드 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TObjectPtr<UAnimInstance> AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TSoftObjectPtr<UAnimMontage> RollMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TSoftObjectPtr<UAnimMontage> DodgeRightMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TSoftObjectPtr<UAnimMontage> DodgeLeftMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TSoftObjectPtr<UAnimMontage> DodgeBackMontage;

// 입력
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	/* 각종 행동 입력 구현 */
	void BasicMove(const FInputActionValue& Value);
	void SpecialMove(const FInputActionValue& Value); // 구르기, 닷지
	void RunWalkSwitch(const FInputActionValue& Value); // 걷기 달리기 전환
	void Look(const FInputActionValue& Value);

private:
	void InputConstructHelper();

protected:
	TObjectPtr<UInputMappingContext> InputMappingContext;

	/* 일반 공격과 특수 공격*/
	TObjectPtr<UInputAction> BasicAttack;
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
