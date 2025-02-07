// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBTService_CalcDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "IVAN/Enemy/IVBlackBoardKeys.h"

UIVBTService_CalcDistance::UIVBTService_CalcDistance()
{
	NodeName = TEXT("Calc Distance");
	Interval = 0.5f;
	bCallTickOnSearchStart = true;
}

void UIVBTService_CalcDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 블랙보드의 타깃 액터와 몬스터의 정보 가져오기
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET_ACTOR));
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();

	// 거리 계산 후 블랙보드에 저장
	if (TargetActor && AIPawn)
	{
		float Distance = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_DISTANCE_TO_TARGET, Distance);
	}
}
