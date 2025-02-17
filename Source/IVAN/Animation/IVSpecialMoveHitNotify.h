// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "IVSpecialMoveHitNotify.generated.h"

/**
 * 보스 몬스터의 특수 이동(데미지 적용됨)이 적용되는 시점을 알리기 위한 노티파이
 * IIVAICharacterSpecialCombat 인터페이스를 구현하는 대상에게 정보를 전달한다. 
 */
UCLASS()
class IVAN_API UIVSpecialMoveHitNotify : public UAnimNotify
{
	GENERATED_BODY()


protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	
};
