// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBossTriggerBox.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Components/ShapeComponent.h"
#include "IVAN/Character/IVPlayerCharacter.h"
#include "IVAN/Enemy/IVBossEnemy.h"
#include "IVAN/Controller/IVPlayerController.h"
#include "IVAN/GameSystem/IVDeathEventSubsystem.h"

AIVBossTriggerBox::AIVBossTriggerBox()
{
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AIVBossTriggerBox::OnOverlapBegin);
	GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &AIVBossTriggerBox::OnOverlapEnd);
}

void AIVBossTriggerBox::BeginPlay()
{
	Super::BeginPlay();
}

void AIVBossTriggerBox::OnPlayerDeath()
{
	if (BossEnemy && PlayerCharacter && BossBattleBGMAudioComponent)
	{
		StopBattleBGM();
	}
}

void AIVBossTriggerBox::OnBossDeath(AActor* DeadMonster)
{
	if (BossEnemy && BossEnemy == DeadMonster)
	{
		StopBattleBGM();
		BossEnemy = nullptr;
	}
}

void AIVBossTriggerBox::StopBattleBGM()
{
	if (BossBattleBGMAudioComponent)
	{
		BossBattleBGMAudioComponent->Stop();
	}
}

void AIVBossTriggerBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 보스 몬스터가 존재해야 진행
	if (!BossEnemy) return;

	// 진입한 액터가 플레이어인 경우 제어 시작
	if (OtherActor->ActorHasTag("Player"))
	{
		PlayerCharacter = Cast<AIVPlayerCharacter>(OtherActor);
	}
	else
	{
		return;
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

	// 전투 BGM 재생
	if (BossBattleBGM)
	{
		BossBattleBGMAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), BossBattleBGM.Get());
	}

	// 플레이어 사망 시 전투 BGM 중지 바인딩
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
	{
		UIVDeathEventSubsystem* LifeEventSubsystem = GameInstance->GetSubsystem<UIVDeathEventSubsystem>();
		if (LifeEventSubsystem)
		{
			// 사망 및 부활 이벤트 서브시스템 등록
			LifeEventSubsystem->PlayerDeathEventDelegate.AddUObject(this, &AIVBossTriggerBox::OnPlayerDeath);
			LifeEventSubsystem->MonsterDeathEventDelegate.AddUObject(this, &AIVBossTriggerBox::OnBossDeath);
		}
	}
}

void AIVBossTriggerBox::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!BossEnemy) return;

	// 플레이어가 범위를 벗어난 경우 HUD위젯 숨김
	if (OtherActor->ActorHasTag("Player"))
	{
		PlayerCharacter = Cast<AIVPlayerCharacter>(OtherActor);
	}
	else
	{
		return;
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

	// 전투 BGM 중지
	StopBattleBGM();

	// 플레이어 사망 시 전투 BGM 중지 바인딩 해제
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
	{
		UIVDeathEventSubsystem* LifeEventSubsystem = GameInstance->GetSubsystem<UIVDeathEventSubsystem>();
		if (LifeEventSubsystem)
		{
			LifeEventSubsystem->PlayerDeathEventDelegate.RemoveAll(this);
			LifeEventSubsystem->MonsterDeathEventDelegate.RemoveAll(this);
		}
	}
}
