// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

AIVBossAIController::AIVBossAIController()
{
}

void AIVBossAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AIVBossAIController::SpecialMoveEnd()
{
	Blackboard->SetValueAsEnum(BBKEY_MONSTER_STATE, static_cast<uint8>(EMonsterState::Chase));
}

void AIVBossAIController::SpecialAttackEnd()
{
	Blackboard->SetValueAsEnum(BBKEY_MONSTER_STATE, static_cast<uint8>(EMonsterState::Chase));
}
