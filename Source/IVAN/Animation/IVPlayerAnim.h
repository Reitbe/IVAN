// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Animation/IVAnimBase.h"
#include "IVAN/Enums/IVCharacterStateEnums.h"
#include "IVPlayerAnim.generated.h"

class UCharacterMovementComponent;
class UCharacterTrajectoryComponent;
class IIIVCharacterStateProvider;
class UIVCharacterStatComponent;

/**
 * 메인 플레이어 캐릭터의 애니메이션
 */

UCLASS()
class IVAN_API UIVPlayerAnim : public UIVAnimBase
{
	GENERATED_BODY()

// 기본
protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
// 캐릭터 움직임 정보 획득을 위한 컴포넌트 목록
protected:
	/* 기본 이동 및 점프 확인용 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	/* 모션 매칭을 위한 캐릭터 궤적 확인용 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UCharacterTrajectoryComponent> TrajectoryComponent;

	/* 특수 상태 및 상태 정보 확인용 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UIVCharacterStatComponent> StatComponent;


// 캐릭터 동작 상태 정보 -> 모션 매칭용 Chooser를 위하여 열거형 사용
public:
	/* 정지, 움직임 상태 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EMovementState MovementState;

	/* 걷기, 달리기 상태 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EGaitState GaitState;

	/* 지상, 점프 후 체공 상태 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EJumpState JumpState;

	/* 타겟팅 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ETargetingState TargetingState;

	/* 구르기, 회피 상태 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ESpecialMovementState SpecialMovementState;

};
