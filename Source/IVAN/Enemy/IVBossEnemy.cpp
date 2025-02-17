// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBossEnemy.h"
#include "IVAN/Attack/IVHitReactionComponent.h"
#include "IVAN/Interface/IIVAIControllerSpecialCombat.h"

AIVBossEnemy::AIVBossEnemy()
{
	MonsterType = EMonsterType::Boss;
}

void AIVBossEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AIVBossEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (HitReactionComponent)
	{
		HitReactionComponent->SetUsingHitReaction(false);
	}
}

void AIVBossEnemy::SpecialMove(AActor* TargetActor) 
{
	// 컨트롤러에게 동작이 종료되었음을 알린다.
	IIIVAIControllerSpecialCombat* AIController = Cast<IIIVAIControllerSpecialCombat>(GetController());
	if (AIController)
	{
		AIController->SpecialMoveEnd();
	}
}

void AIVBossEnemy::SpecialAttack(AActor* TargetActor)
{
	// 컨트롤러에게 동작이 종료되었음을 알린다.
	IIIVAIControllerSpecialCombat* AIController = Cast<IIIVAIControllerSpecialCombat>(GetController());
	if (AIController)
	{
		AIController->SpecialAttackEnd();
	}
}

void AIVBossEnemy::OnNotifySpecialMoveHit()
{
	// 구체적인 내용은 세부 보스 클래스에서 구현
}

void AIVBossEnemy::OnNotifySpecialAttackHit()
{
	// 구체적인 내용은 세부 보스 클래스에서 구현
}
