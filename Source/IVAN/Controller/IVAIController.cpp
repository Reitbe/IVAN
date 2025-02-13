// Fill out your copyright notice in the Description page of Project Settings.


#include "IVAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "IVAN/Interface/IIVMonsterComponentProvider.h"
#include "IVAN/Stat/IVMonsterStatComponent.h"

AIVAIController::AIVAIController()
{
	// AI 인지 컴포넌트 생성
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	// 시각 인지 설정 -> 세부 사항은 블루프린트에서 설정
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	// 인지-동작 바인딩
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AIVAIController::OnPerceptionUpdated);
}

void AIVAIController::RunAI()
{
	// 에디터에서 지정한 블랙보드 에셋을 현 AI컨트롤러의 블랙보드로 설정
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (UseBlackboard(BlackboardAsset, BlackboardComp))
	{
		bool RunResult = RunBehaviorTree(BehaviorTreeAsset);
	}
}

void AIVAIController::StopAI()
{
	// 행동트리 정지용
	BrainComponent->StopLogic("AI Stop");

	// 조종 해제
	UnPossess();
}

void AIVAIController::SetDead()
{
	// 행동트리 사용 중지
	StopAI();
	
	// 사망 정보를 블랙보드에 전달
	Blackboard->SetValueAsEnum(BBKEY_MONSTER_STATE, static_cast<uint8>(EMonsterState::Dead));
	Blackboard->ClearValue(BBKEY_TARGET_ACTOR);
}

void AIVAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 행동트리 실행
	RunAI();

	// 몬스터 사망 이벤트 바인딩
	IIIVMonsterComponentProvider* MonsterProvider = Cast<IIIVMonsterComponentProvider>(InPawn);
	if (MonsterProvider)
	{
		MonsterProvider->GetMonsterStatComponent()->OnMonsterDeathLocalEvent.AddUObject(this, &AIVAIController::SetDead);
	}
}

void AIVAIController::OnPerceptionUpdated(AActor* UpdatedActor, FAIStimulus UpdatedStimulus)
{
	// 감지된 감각이 시각이며 성공적으로 감지를 완료한 경우
	if (UpdatedStimulus.Type == UAISense::GetSenseID<UAISense_Sight>() && UpdatedStimulus.WasSuccessfullySensed())
	{
		// 추후 게임플레이 태그를 사용하여 플레이어 구분. 현재는 액터 태그 사용
		if (UpdatedActor->ActorHasTag("Player"))
		{
			Blackboard->SetValueAsObject(BBKEY_TARGET_ACTOR, UpdatedActor);
		}
	}
}

void AIVAIController::OnAttackEnd()
{
	Blackboard->SetValueAsEnum(BBKEY_MONSTER_STATE, static_cast<uint8>(EMonsterState::Chase));
}

void AIVAIController::OnHit(AActor* Attacker)
{
	Blackboard->SetValueAsEnum(BBKEY_MONSTER_STATE, static_cast<uint8>(EMonsterState::HitStunned));

	// 공격자를 타깃 액터로 지정
	if (Attacker->ActorHasTag("Player"))
	{
		Blackboard->SetValueAsObject(BBKEY_TARGET_ACTOR, Attacker);
	}
}

void AIVAIController::OnHitEnd()
{
	Blackboard->SetValueAsEnum(BBKEY_MONSTER_STATE, static_cast<uint8>(EMonsterState::Chase));
}
