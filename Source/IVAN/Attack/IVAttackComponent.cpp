// Fill out your copyright notice in the Description page of Project Settings.


#include "IVAttackComponent.h"
#include "IVAN/Item/IVWeapon.h"
#include "IVAN/Interface/IIVWeaponInterface.h"

// Sets default values for this component's properties
UIVAttackComponent::UIVAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxComboCount = 1;
	ComboCount = 0;
	ComboResetTime = 1.0f;
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

	// 무기의 콤보 정보 갱신
	IIIVWeaponInterface* WeaponInterface = Cast<IIIVWeaponInterface>(WeaponInstance);
	if (WeaponInterface)
	{
		MaxComboCount = WeaponInterface->GetMaxComboCount();
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
			}
			ComboCount = (ComboCount < MaxComboCount - 1) ? ComboCount + 1 : 0; // 콤보 카운트 증가
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
}

