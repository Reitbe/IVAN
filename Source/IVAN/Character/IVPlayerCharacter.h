// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Character/IVCharacterBase.h"
#include "InputActionValue.h"
#include "IVPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UInputComponent;
class USpringArmComponent;
class UCameraComponent;

/**
 * 플레이어가 조종할 캐릭터
 */
UCLASS()
class IVAN_API AIVPlayerCharacter : public AIVCharacterBase
{
	GENERATED_BODY()

// 기본
public:
	AIVPlayerCharacter();

public:
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

protected:
	TObjectPtr<APlayerController> PlayerController;


// 입력
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	/* 각종 행동 입력 구현 함수*/
	void BasicMove(const FInputActionValue& Value);
	void SpecialMove(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

private:
	void InputConstructHelper();

protected:
	TObjectPtr<UInputMappingContext> InputMappingContext;

	/* 일반 공격과 특수 공격*/
	TObjectPtr<UInputAction> BasicAttack;
	TObjectPtr<UInputAction> SpecialAttack;

	/* 2d 평면상의 일반 움직임과 구르기, 점프, 전력질주용 움직임. 그리고 시선*/
	TObjectPtr<UInputAction> BasicMovement;
	TObjectPtr<UInputAction> SpecialMovement;
	TObjectPtr<UInputAction> LookAction;

	/* 특수 상호작용*/
	TObjectPtr<UInputAction> TargetAction;
	TObjectPtr<UInputAction> InteractAction;
	TObjectPtr<UInputAction> DefendAction;


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
