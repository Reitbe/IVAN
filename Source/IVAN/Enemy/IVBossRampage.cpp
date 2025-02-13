// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBossRampage.h"
#include "IVAN/Attack/IVAttackRange.h"

AIVBossRampage::AIVBossRampage()
{
	RightHandAttackRange = CreateDefaultSubobject<UIVAttackRange>(TEXT("RightHandAttackRange"));
	LeftHandAttackRange = CreateDefaultSubobject<UIVAttackRange>(TEXT("LeftHandAttackRange"));
}

void AIVBossRampage::BeginPlay()
{
}

void AIVBossRampage::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttackRanges.Empty(); // 기존 공격 범위 초기화
	AttackRanges.Add(RightHandAttackRange);
	AttackRanges.Add(LeftHandAttackRange);

	// 소켓에 콜라이더 부착
	FName RampageRightHandSocket(TEXT("Rampage_R_Hand"));
	if (GetMesh()->DoesSocketExist(RampageRightHandSocket))
	{
		RightHandAttackRange->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RampageRightHandSocket);
	}

	FName RampageLeftHandSocket(TEXT("Rampage_L_Hand"));
	if (GetMesh()->DoesSocketExist(RampageLeftHandSocket))
	{
		LeftHandAttackRange->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RampageLeftHandSocket);
	}
}

void AIVBossRampage::SpecialMove(AActor* TargetActor)
{
	if (SpecialMoveMontage && AnimInstance)
	{
		// 특수 이동 몽타주 재생
		AnimInstance->Montage_PlayWithBlendIn(SpecialMoveMontage, 1.0f);

		// 몽타주 종료 시 대리자 호출
		FOnMontageEnded MontageEndDelegate;
		MontageEndDelegate.BindLambda([this, TargetActor](UAnimMontage* Montage, bool bInterrupted)
			{
				OnSpecialMoveEnd(TargetActor);
			});
		AnimInstance->Montage_SetEndDelegate(MontageEndDelegate);
	}
}

void AIVBossRampage::SpecialAttack(AActor* TargetActor)
{
	if (SpecialAttackMontage && AnimInstance)
	{
		// 특수 이동 몽타주 재생
		AnimInstance->Montage_PlayWithBlendIn(SpecialAttackMontage, 1.0f);

		// 몽타주 종료 시 대리자 호출
		FOnMontageEnded MontageEndDelegate;
		MontageEndDelegate.BindLambda([this, TargetActor](UAnimMontage* Montage, bool bInterrupted)
			{
				OnSpecialAttackEnd(TargetActor);
			});
		AnimInstance->Montage_SetEndDelegate(MontageEndDelegate);
	}
}

void AIVBossRampage::OnSpecialMoveEnd(AActor* TargetActor)
{
	// 행동 종료를 컨트롤러에 알리는 BossEnemy의 SpecialMove 함수를 호출
	Super::SpecialMove(TargetActor);
}

void AIVBossRampage::OnSpecialAttackEnd(AActor* TargetActor)
{
	// 행동 종료를 컨트롤러에 알리는 BossEnemy의 SpecialAttack 함수를 호출
	Super::SpecialAttack(TargetActor);
}
