// Fill out your copyright notice in the Description page of Project Settings.


#include "IVWeaponHitDetection.h"
#include "IVAN/Item/IVWeapon.h"
#include "IVAN/Interface/IIVWeaponProvider.h"

void UIVWeaponHitDetection::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	// 노티파이 시작 시 몽타주 오너가 소유한 무기를 가져온다
	if (IIIVWeaponProvider* WeaponProvider = Cast<IIIVWeaponProvider>(MeshComp->GetOwner()))
	{
		Weapon = WeaponProvider->GetWeapon().Get();
	}
}

void UIVWeaponHitDetection::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	// 노티파이 틱마다 무기의 충돌 검사 및 데미지 전달을 수행한다
	if (Weapon)
	{
		Weapon->HitDetection();
	}
}

void UIVWeaponHitDetection::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// 노티파이 종료 시 무기를 초기화한다
	if (Weapon)
	{
		Weapon->ClearHitActors();
		Weapon = nullptr;
	}
}
