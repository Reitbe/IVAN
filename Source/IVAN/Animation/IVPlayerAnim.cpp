﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "IVPlayerAnim.h"
#include "IVAN/Interface/IIVCharacterComponentProvider.h"
#include "IVAN/Stat/IVCharacterStatComponent.h" 

void UIVPlayerAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	// ABP소유자의 컴포넌트 획득
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

	// 캐릭터의 상태 정보를 업데이트
	if (StatComponent)
	{
		MovementState = StatComponent->GetCharacterMovementState();
		GaitState = StatComponent->GetCharacterGaitState();
		JumpState = StatComponent->GetCharacterJumpState();
		TargetingState = StatComponent->GetCharacterTargetingState();
		SpecialMovementState = StatComponent->GetCharacterSpecialMovementState();
	}
}
