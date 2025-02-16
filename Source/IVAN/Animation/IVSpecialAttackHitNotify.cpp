// Fill out your copyright notice in the Description page of Project Settings.


#include "IVSpecialAttackHitNotify.h"
#include "IVAN/Interface/IIVAICharacterSpecialCombat.h"

void UIVSpecialAttackHitNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		// 특수 공격 히트 인터페이스를 통해 정보 전달
		IIIVAICharacterSpecialCombat* SpecialCombatInterface = Cast<IIIVAICharacterSpecialCombat>(MeshComp->GetOwner());
		if (SpecialCombatInterface)
		{
			SpecialCombatInterface->OnNotifySpecialAttackHit();
		}
	}

}
