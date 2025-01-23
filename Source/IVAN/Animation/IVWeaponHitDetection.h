// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "IVWeaponHitDetection.generated.h"

class AIVWeapon;

/**
 * 공격 애니메이션 몽타주 재생 중
 * 무기의 충돌 검사를 제어하는 클래스
 */
UCLASS()
class IVAN_API UIVWeaponHitDetection : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
private:
	/* 몽타주 적용 대상이 소유하고 있는 무기 */
	TObjectPtr<AIVWeapon> Weapon;

};
