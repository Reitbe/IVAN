// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "IVAN/Enemy/IVBlackBoardKeys.h"
#include "IVAN/Interface/IIVAIControllerBasicCombat.h"
#include "IVAN/Interface/IIVAICharacterBasicCombat.h"
#include "IVAIController.generated.h"

class UBehaviorTree;
class UBlackboardData;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;

/**
 * 일반 몬스터 AI의 움직임을 제어하는 기본 클래스. 현재는 스켈레톤만 사용하고 있다.
 * 보스몬스터는 별도의 클래스로 분리하여 사용한다.
 */
UCLASS()
class IVAN_API AIVAIController : public AAIController, public IIIVAIControllerBasicCombat
{
	GENERATED_BODY()


// 기본
public:
	AIVAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;


// AI 동작 기본
protected:
	void RunAI();
	void StopAI();
	void SetDead();


// AI 인지 컴포넌트
protected:
	/* 행동 정보 업데이트시 호출*/
	UFUNCTION()
	void OnPerceptionUpdated(AActor* UpdatedActor, FAIStimulus UpdatedStimulus);

	/* 인지용 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	/* 시각 정보 */
	TObjectPtr<UAISenseConfig_Sight> SightConfig;


// 행동 트리 & 블랙보드
protected:
	/* 행동 트리 - 에디터에서 지정 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	/* 블랙보드 - 에디터에서 지정 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	TObjectPtr<UBlackboardData> BlackboardAsset;


// IIIVAIControllerBasicCombat 인터페이스 구현
public:
	virtual void OnAttackEnd() override;
	virtual void OnHit(AActor* Attacker) override;
	virtual void OnHitEnd() override;
};
