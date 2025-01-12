// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IVAN/IVCharacterStateEnums.h"
#include "IVCharacterStatComponent.generated.h"

class UCharacterMovementComponent;

/*
* ĳ���� ������ �����ϴ� ������Ʈ.
* ������ ����, ü��(�̱���) ���� �����Ѵ�.
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IVAN_API UIVCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UIVCharacterStatComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// ĳ���� ���� ���� ����
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
	
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

// ĳ���� ���� ���� �� ȹ��
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
