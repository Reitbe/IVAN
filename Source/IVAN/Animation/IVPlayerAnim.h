// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Animation/IVAnimBase.h"
#include "IVAN/IVCharacterStateEnums.h"
#include "IVPlayerAnim.generated.h"

class UCharacterMovementComponent;
class UCharacterTrajectoryComponent;
class IIIVCharacterStateProvider;
class UIVCharacterStatComponent;

/**
 * �÷��̾� ĳ������ �ִϸ��̼��� ����Ѵ�.
 */

UCLASS()
class IVAN_API UIVPlayerAnim : public UIVAnimBase
{
	GENERATED_BODY()

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
// ĳ���� ���� ȹ���� ���� ������Ʈ
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UCharacterTrajectoryComponent> TrajectoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UIVCharacterStatComponent> StatComponent;


// ĳ���� ���� ����
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EMovementState MovementState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EGaitState GaitState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EJumpState JumpState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ETargetingState TargetingState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ESpecialMovementState SpecialMovementState;

};
