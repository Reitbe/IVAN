// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBossTriggerBox.h"
#include "Components/ShapeComponent.h"
#include "IVAN/Character/IVPlayerCharacter.h"
#include "IVAN/Enemy/IVBossEnemy.h"
#include "IVAN/Controller/IVPlayerController.h"

AIVBossTriggerBox::AIVBossTriggerBox()
{
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AIVBossTriggerBox::OnOverlapBegin);
	GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &AIVBossTriggerBox::OnOverlapEnd);
}

void AIVBossTriggerBox::BeginPlay()
{
	Super::BeginPlay();
}

void AIVBossTriggerBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 진입한 액터가 플레이어인 경우 제어 시작
	if (OtherActor->ActorHasTag("Player"))
	{
		PlayerCharacter = Cast<AIVPlayerCharacter>(OtherActor);
	}

	// 플레이어 컨트롤러의 HUD위젯에 보스 정보를 표시
	if (PlayerCharacter) 
	{
		AIVPlayerController* PlayerController = Cast<AIVPlayerController>(PlayerCharacter->GetController());
		if (PlayerController && BossEnemy.IsValid())
		{
			PlayerController->ShowBossStatWidget(BossEnemy.Get());
		}
	}
	PlayerCharacter = nullptr;
}

void AIVBossTriggerBox::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 플레이어가 범위를 벗어난 경우 HUD위젯 숨김
	if (OtherActor->ActorHasTag("Player"))
	{
		PlayerCharacter = Cast<AIVPlayerCharacter>(OtherActor);
	}

	if (PlayerCharacter)
	{
		AIVPlayerController* PlayerController = Cast<AIVPlayerController>(PlayerCharacter->GetController());
		if (PlayerController && BossEnemy.IsValid())
		{
			PlayerController->HideBossStatWidget();
		}
	}
	PlayerCharacter = nullptr;
}
