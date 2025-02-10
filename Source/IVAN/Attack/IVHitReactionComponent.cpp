// Fill out your copyright notice in the Description page of Project Settings.


#include "IVHitReactionComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "IVAN/Interface/IIVHitReactionInterface.h"

UIVHitReactionComponent::UIVHitReactionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 변수 초기화
	bIsUsingHitReaction = true;
	bIsUsingRagdoll = false;
}

void UIVHitReactionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UIVHitReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// AActor의 TakeDamage 정보를 그대로 가져와서 사용
bool UIVHitReactionComponent::ComputeHitAngle(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!bIsUsingHitReaction || !GetOwner()) return false; // 피격 리액션 사용 여부 및 컴포넌트 연결 확인

	AActor* Owner = GetOwner(); // 피격 대상
	float Angle = 0.0f;	 

	// 공격 방향 계산
	if(DamageCauser)
	{
		// 이후 연산을 위한 벡터 정규화
		FVector CharacterFoward = Owner->GetActorForwardVector().GetSafeNormal(); // 이미 정규화 되어있긴 하다
		FVector HitDirection = (DamageCauser->GetActorLocation() - Owner->GetActorLocation()).GetSafeNormal();

		// 캐릭터 정면 기준 공격 방향 각도
		/*
		* DotProduct : 
		*	내적을 통해 두 벡터 사이 각도 계산. A dot B = |A||B|cos(theta)에서 입력 벡터가 정규화 된 상태이므로
		*	A dot B = cos(theta)가 된다. cos(theta) 범위는 -1 ~ 1이다.
		* Acos :
		*	내적을 통해 얻은 cos(theta)를 역함수인 Acos를 사용해 각도로 변환한다. 이 때 각도는 라디안이다.
		* RadiansToDegrees :
		*	앞서 얻은 라디안을 각도로 변환한다.
		*/
		Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(CharacterFoward, HitDirection)));

		// 공격 방향 좌우 판단
		/*
		* CrossProduct :
		*	캐릭터 정면을 기준으로 피격 방향과의 외적을 진행한다. 오른손 법칙에 따라
		*	외적값이 양수라면 공격자가 오른 쪽, 음수라면 왼쪽에서 공격을 받았다고 판단한다.
		* Angle : 
		*	정면 +-0도, 후면 +-180도를 기준으로 오른쪽(0~180), 왼쪽(-0~-180)범위이다.
		*/
		FVector CrossProduct = FVector::CrossProduct(CharacterFoward, HitDirection);
		Angle *= (CrossProduct.Z > 0.0f) ? 1.0f : -1.0f;
	}

	// 방향에 따른 애니메이션 선택
	if (AnimInstance)
	{
		PlayHitReactionMontage(Angle);
	}
	return true;
}

void UIVHitReactionComponent::PlayHitReactionMontage(float Angle)
{
	if (!AnimInstance) return;

	UAnimMontage* HitReactionMontage = nullptr; // 재생할 몽타주

	// 각 90도씩 4방향 범위 판단 후 재생할 몽타주 선택
	if (Angle >= -45.0f && Angle < 45.0f)
	{
		HitReactionMontage = FrontHitMontage; // 정면
	}
	else if (Angle >= 45.0f && Angle < 135.0f)
	{
		HitReactionMontage = RightHitMontage; // 오른쪽
	}
	else if (Angle >= -135.0f && Angle < -45.0f)
	{
		HitReactionMontage = LeftHitMontage; // 왼쪽
	}
	else if (Angle >= 135.0f || Angle < -135.0f)
	{
		HitReactionMontage = BackHitMontage; // 뒤쪽
	}

	AnimInstance->Montage_Play(HitReactionMontage); // 몽타주 재생

	// 피격 애니메이션 종료 시 오너의 피격 리액션 종료
	FOnMontageBlendingOutStarted BlendingOutDelegate;
	BlendingOutDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			IIIVHitReactionInterface* HitReactionInterface = Cast<IIIVHitReactionInterface>(GetOwner());
			if (HitReactionInterface)
			{
				HitReactionInterface->EndHitReaction();
			}
		});
	AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, HitReactionMontage);
}

void UIVHitReactionComponent::PlayDeathMontage()
{
	if (!AnimInstance || !DeathMontage) return;
	AnimInstance->Montage_Play(DeathMontage); // 사망 몽타주 재생

	// 몽타주 종료 시 레그돌로 전환
	FOnMontageBlendingOutStarted BlendingOutDelegate;
	BlendingOutDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			IIIVHitReactionInterface* HitReactionInterface = Cast<IIIVHitReactionInterface>(GetOwner());
			if (HitReactionInterface)
			{
				HitReactionInterface->EndDeathReaction();
			}
		});
	AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, DeathMontage);
}

