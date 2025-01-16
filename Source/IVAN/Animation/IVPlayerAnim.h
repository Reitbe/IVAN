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
 * 플레이어 캐릭터의 애니메이션을 담당한다.
 */

UCLASS()
class IVAN_API UIVPlayerAnim : public UIVAnimBase
{
	GENERATED_BODY()

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
// 캐릭터 움직임 정보 획득을 위한 컴포넌트
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UCharacterTrajectoryComponent> TrajectoryComponent;

	/* 캐릭터 상태 정보 및 움직임 정보 획득용 */
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
