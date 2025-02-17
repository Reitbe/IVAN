// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBossRampage.h"
#include "MotionWarpingComponent.h" 
#include "Kismet/KismetMathLibrary.h"
#include "IVAN/Attack/IVAttackRange.h"
#include "IVAN/Stat/IVMonsterStatComponent.h"
#include "IVAN/Attack/IVHitReactionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


AIVBossRampage::AIVBossRampage()
{
	// 모션 워핑
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	// 공격 범위
	RightHandAttackRange = CreateDefaultSubobject<UIVAttackRange>(TEXT("RightHandAttackRange"));
	LeftHandAttackRange = CreateDefaultSubobject<UIVAttackRange>(TEXT("LeftHandAttackRange"));

	// 특수 동작 데미지
	SpecialMoveDamage = 50.0f;
	SpecialAttackDamage = 30.0f;
}

void AIVBossRampage::BeginPlay()
{
	Super::BeginPlay();
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
	/*
	* 캐릭터의 회전과 이동을 위하여 Motion Warping과 LaunchCharacter를 동시에 사용하고자 했으나
	* 충돌로 인한 이동 불가, 위치 이동 반복 등의 문제가 발생하였기에 점프 준비와 점프 실행으로 동작을 나누었다.
	* 점프 준비는 Motion Warping을 사용하여 플레이어를 바라보도록 회전을 진행하고
	* 점프 실행은 LaunchCharacter를 사용하여 플레이어의 위치로 이동을 진행한다.
	*/

	if (SpecialMoveMontage_FirstPart && SpecialMoveMontage_SecondPart && AnimInstance)
	{
		// 모션 워핑 대상 지정
		MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(FName("Player"), TargetActor->GetActorTransform());

		// 1차 애니메이션 재생 
		AnimInstance->Montage_Play(SpecialMoveMontage_FirstPart);

		// 1차 몽타주 재생 완료 시 
		FOnMontageEnded FirstMontageEndDelegate;
		FirstMontageEndDelegate.BindLambda([this, TargetActor](UAnimMontage* Montage, bool bInterrupted)
			{
				// 2차 몽타주 재생 && 캐릭터 발사
				StartLaunchCharacter(TargetActor);
				AnimInstance->Montage_Play(SpecialMoveMontage_SecondPart, 1.0f);

				// 2차 애니메이션 재생 완료 시
				FOnMontageEnded SecondMontageEndDelegate;
				SecondMontageEndDelegate.BindLambda([this, TargetActor](UAnimMontage* Montage, bool bInterrupted)
					{
						if (Montage == SpecialMoveMontage_SecondPart) 
						{
							// 온전히 SpecialMove가 종료되었음을 전달한다.
							OnSpecialMoveEnd(TargetActor);
						}
					});
				AnimInstance->Montage_SetEndDelegate(SecondMontageEndDelegate, SpecialMoveMontage_SecondPart);

			});
		AnimInstance->Montage_SetEndDelegate(FirstMontageEndDelegate);

	}
}

void AIVBossRampage::SpecialAttack(AActor* TargetActor)
{
	if (SpecialAttackMontage && AnimInstance)
	{
		AnimInstance->Montage_PlayWithBlendIn(SpecialAttackMontage, 1.0f);

		FOnMontageEnded MontageEndDelegate;
		MontageEndDelegate.BindLambda([this, TargetActor](UAnimMontage* Montage, bool bInterrupted)
			{
				OnSpecialAttackEnd(TargetActor);
			});
		AnimInstance->Montage_SetEndDelegate(MontageEndDelegate);
	}
}

void AIVBossRampage::OnNotifySpecialMoveHit() 
{
	// 땅을 내려찍는 시점에 범위 데미지를 전달.
	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), SpecialMoveDamage, SpecialMoveDamage / 4.0f, GetActorLocation(), 100.0f, 300.0f, 1.0f, nullptr, TArray<AActor*>(), this, GetController());
}

void AIVBossRampage::OnNotifySpecialAttackHit() 
{
	// 땅을 내려찍는 시점에 범위 데미지를 전달.
	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), SpecialAttackDamage, SpecialMoveDamage / 4.0f, GetActorLocation(), 100.0f, 400.0f, 1.0f, nullptr, TArray<AActor*>(), this, GetController());
}

void AIVBossRampage::OnSpecialMoveEnd(AActor* TargetActor)
{
	// 행동 종료를 컨트롤러에 알리는 SpecialMove 함수를 호출(Super로 호출 금지!!)
	AIVBossEnemy::SpecialMove(TargetActor);
}

void AIVBossRampage::OnSpecialAttackEnd(AActor* TargetActor)
{
	// 행동 종료를 컨트롤러에 알리는 SpecialAttack 함수를 호출(Super로 호출 금지!!)
	AIVBossEnemy::SpecialAttack(TargetActor);
}

void AIVBossRampage::StartLaunchCharacter(AActor* TargetActor)
{
	// 각 대상의 위치를 구한다.
	FVector StartLocation = GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();

	// 몬스터가 가장 높게 뛰어오를 때의 높이를 지정한다
	float ApexHeight = FMath::Max(StartLocation.Z, TargetLocation.Z) + 150.0f;

	/*
	* [수직운동]
	* Gravity :
	*	보스몬스터에 적용되는 중력가속도이다. GetGravityZ()가 음수값을 반환하므로 연산을 위해 -1을 곱해준다.
	* TimeToApex :
	*	최대 지점까지 도달하기 위한 시간이다. 등가속도 공식 중  s = (v0 * t) + ((1/2) * g * t^2)을 사용한다.
	*	초기 속도는 없기에 v0 = 0, s = 수직이동거리 h, g = 중력가속도 Gravity이므로 t = sqrt(2 * h / g)이다.
	* TotalTime :
	*	TimeToApex는 시작지점에서 최대 높이까지 걸리는 시간이므로 다시 땅으로 내려오는 시간까지 고려하여 2배를 한다.
	*/
	float Gravity = GetWorld()->GetGravityZ() * -1.0f;
	float TimeToApex = FMath::Sqrt(2 * (ApexHeight - StartLocation.Z) / Gravity); 
	float TotalTime = TimeToApex * 2.0f; 

	/*
	* [수평운동]
	* 수평운동에서는 외부 요인이 없으므로 등속 운동을 한다고 가정한다. 단순하게 거리 = 속도 * 시간을 사용한다.
	* 수평 속도 = (목표지점 - 시작지점) / 총 시간이다. 
	*/
	FVector HorizontalVelocity = (TargetLocation - StartLocation) / TotalTime;

	// 수평이동과 수직이동을 합하여 최종 발사 속도를 구한다.
	float VerticalVelocity = Gravity * TimeToApex;
	FVector LaunchVelocity = HorizontalVelocity + FVector(0, 0, VerticalVelocity);

	// 캐릭터 발사!!! 
	LaunchCharacter(LaunchVelocity, true, true);
}
