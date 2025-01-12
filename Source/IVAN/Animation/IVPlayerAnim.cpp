// Fill out your copyright notice in the Description page of Project Settings.


#include "IVPlayerAnim.h"
#include "IVAN/Interface/IIVCharacterComponentProvider.h"
#include "IVAN/Stat/IVCharacterStatComponent.h" 

void UIVPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UIVPlayerAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	// ĳ���Ϳ� ���� ������Ʈ�� �����Ϳ� ����
	AActor* Owner = GetOwningActor();
	if (Owner->Implements<UIIVCharacterComponentProvider>())
	{
		IIIVCharacterComponentProvider* Provider = Cast<IIIVCharacterComponentProvider>(Owner);
		MovementComponent = Provider->GetCharacterMovementComponent();
		TrajectoryComponent = Provider->GetCharacterTrajectoryComponent();
		StatComponent = Provider->GetCharacterStatComponent(); 
	}
}

void UIVPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// ĳ������ ���� ������ ������Ʈ
	if (StatComponent)
	{
		MovementState = StatComponent->GetCharacterMovementState();
		GaitState = StatComponent->GetCharacterGaitState();
		JumpState = StatComponent->GetCharacterJumpState();
		TargetingState = StatComponent->GetCharacterTargetingState();
		SpecialMovementState = StatComponent->GetCharacterSpecialMovementState();
	}
}
