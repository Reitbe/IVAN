// Fill out your copyright notice in the Description page of Project Settings.


#include "IVAttackComponent.h"
#include "GameFramework/Character.h"
#include "IVAN/Item/IVWeapon.h"
#include "IVAN/Interface/IIVWeaponInterface.h"
#include "IVAN/Interface/IIVAttackEndInterface.h"
#include "IVAN/Attack/IVAttackRange.h"

// Sets default values for this component's properties
UIVAttackComponent::UIVAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxComboCount = 1;
	ComboCount = 0;
	ComboResetTime = 2.0f;
}

void UIVAttackComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UIVAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UIVAttackComponent::SetWeapon(TObjectPtr<AIVWeapon> NewWeapon)
{
	// 전달 받은 무기를 현재 무기로 설정
	WeaponInstance = NewWeapon;
	if (!WeaponInstance) // 무기 해제 시 WeaponInstance를 nullptr로 설정
	{
		return;
	}

	// 무기 정보 갱신
	IIIVWeaponInterface* WeaponInterface = Cast<IIIVWeaponInterface>(WeaponInstance);
	if (WeaponInterface)
	{
		// 캐릭터의 컨트롤러를 무기에 전달
		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
		if (OwnerCharacter)
		{
			WeaponInterface->SetOwnerController(OwnerCharacter->GetController());
		}
		MaxComboCount = WeaponInterface->GetMaxComboCount();
	}
}

void UIVAttackComponent::ProvideOwnerAttackRanges(const TArray<UIVAttackRange*> AttackRanges)
{
	if (WeaponInstance)
	{
		IIIVWeaponInterface* WeaponInterface = Cast<IIIVWeaponInterface>(WeaponInstance);
		if (WeaponInterface)
		{
			WeaponInterface->SetOwnerAttackRanges(AttackRanges);
		}
	}
}

void UIVAttackComponent::Attack(FBaseDamageStat DamageStat)
{
	if (WeaponInstance)
	{	
		IIIVWeaponInterface* WeaponInterface = Cast<IIIVWeaponInterface>(WeaponInstance);
		if (WeaponInterface)
		{
			// 무기에 데미지 계산을 위한 캐릭터 데미지 스탯 전달
			WeaponInterface->SetOwnerDamageStat(DamageStat);

			// 콤보 몽타주 재생
			UAnimMontage* ComboMontage = WeaponInterface->GetComboMontage(ComboCount);
			if (ComboMontage && AnimInstance)
			{
				AnimInstance->Montage_Play(ComboMontage); 
				
				// 공격 도중 피격	상태로 전환되는 경우를 위한 대리자
				FOnMontageEnded MontageEndDelegate;
				MontageEndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
					{
						IIIVAttackEndInterface* AttackEndInterface = Cast<IIIVAttackEndInterface>(GetOwner());
						if (AttackEndInterface)
						{
							AttackEndInterface->AttackEnd(false); // 2차 검사이므로 False 전달
						}
					});
				AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, ComboMontage);
			}
			ComboCount = (ComboCount < MaxComboCount - 1) ? ComboCount + 1 : 0; // 콤보 카운트 증가
		}
	}
	else
	{
		// 공격을 시도했는데 무기가 없어서 공격이 취소된 경우
		IIIVAttackEndInterface* AttackEndInterface = Cast<IIIVAttackEndInterface>(GetOwner());
		if (AttackEndInterface)
		{
			AttackEndInterface->AttackCancel(); // 공격 취소
		}
	}
	GetWorld()->GetTimerManager().ClearTimer(ComboTimer); // 콤보 타이머 초기화
}

void UIVAttackComponent::AttackEnd()
{
	// 콤보 카운트 리셋 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(ComboTimer, this, &UIVAttackComponent::ResetCombo, ComboResetTime, false);
}

void UIVAttackComponent::ResetCombo()
{
	ComboCount = 0;
	IIIVAttackEndInterface* AttackEndInterface = Cast<IIIVAttackEndInterface>(GetOwner());
	if (AttackEndInterface)
	{
		AttackEndInterface->ResetComboEnd(); // 1차 검사이므로 True 전달
	}
}

