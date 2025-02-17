// Fill out your copyright notice in the Description page of Project Settings.


#include "IVAttackEndNotify.h"
#include "IVAN/Interface/IIVAttackEndInterface.h"

void UIVAttackEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		// 공격 종료 인터페이스를 통해 정보 전달
		IIIVAttackEndInterface* AttackEndInterface = Cast<IIIVAttackEndInterface>(MeshComp->GetOwner());
		if (AttackEndInterface)
		{
			AttackEndInterface->AttackEnd(true); // 1회차 검사이므로 true 전달
		}
	}
}
