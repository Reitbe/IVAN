// Fill out your copyright notice in the Description page of Project Settings.


#include "IVSimpleStatHUD.h"
#include "Kismet/GameplayStatics.h"
#include "IVAN/Enemy/IVBossEnemy.h"
#include "IVAN/Interface/IIVCharacterComponentProvider.h"
#include "IVAN/Interface/IIVMonsterComponentProvider.h"
#include "IVAN/UI/IVBaseStatBar.h"
#include "IVAN/UI/IVSimpleStatWidget.h"
#include "IVAN/UI/IVSimpleBossStatWidget.h"
#include "IVAN/Stat/IVCharacterStatComponent.h"
#include "IVAN/Stat/IVMonsterStatComponent.h"
#include "IVAN/GameSystem/IVDeathEventSubsystem.h"

AIVSimpleStatHUD::AIVSimpleStatHUD()
{
	PrimaryActorTick.bCanEverTick = true;

	// HUD에 표시할 위젯 클래스
	static ConstructorHelpers::FClassFinder<UIVSimpleStatWidget> PlayerStatWidgetClassFinder
	(TEXT("/Game/Widget/WBP_SimpleStat.WBP_SimpleStat_C"));
	if (PlayerStatWidgetClassFinder.Class)
	{
		PlayerStatWidgetClass = PlayerStatWidgetClassFinder.Class;
	}

	// 사망 시 표시할 위젯 클래스
	static ConstructorHelpers::FClassFinder<UUserWidget> DeathWidgetClassFinder
	(TEXT("/Game/Widget/WBP_YouDie.WBP_YouDie_C"));
	if (DeathWidgetClassFinder.Class)
	{
		DeathWidgetClass = DeathWidgetClassFinder.Class;
	}

	// 보스 스텟 표시용 위젯 클래스
	static ConstructorHelpers::FClassFinder<UUserWidget> BossStatWidgetClassFinder
	(TEXT("/Game/Widget/WBP_SimpleBossStat.WBP_SimpleBossStat_C"));
	if (BossStatWidgetClassFinder.Class)
	{
		BossStatWidgetClass = BossStatWidgetClassFinder.Class;
	}

	// 보스 클리어 위젯 클래스
	static ConstructorHelpers::FClassFinder<UUserWidget> BossClearWidgetClassFinder
	(TEXT("/Game/Widget/WBP_BossClear.WBP_BossClear_C"));
	if (BossClearWidgetClassFinder.Class)
	{
		BossClearWidgetClass = BossClearWidgetClassFinder.Class;
	}

	// 타겟팅 대상에 부착할 마커 클래스
	static ConstructorHelpers::FClassFinder<UUserWidget> TargetMarkerWidgetClassFinder
	(TEXT("/Game/Widget/WBP_TargetMarker.WBP_TargetMarker_C"));
	if (TargetMarkerWidgetClassFinder.Class)
	{
		TargetMarkerWidgetClass = TargetMarkerWidgetClassFinder.Class;
	}

}

void AIVSimpleStatHUD::BeginPlay()
{
	Super::BeginPlay();

	// 사망 시 표시할 위젯 생성
	if (DeathWidgetClass)
	{
		DeathWidget = CreateWidget<UUserWidget>(GetWorld(), DeathWidgetClass);
		DeathWidget->AddToViewport(4); // 앞에 배치
		DeathWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 보스 클리어 위젯 생성
	if (BossClearWidgetClass)
	{
		BossClearWidget = CreateWidget<UUserWidget>(GetWorld(), BossClearWidgetClass);
		BossClearWidget->AddToViewport(3); // 중간에 배치
		BossClearWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 보스 스텟 표시용 위젯 생성
	if (BossStatWidgetClass)
	{
		BossStatWidget = CreateWidget<UIVSimpleBossStatWidget>(GetWorld(), BossStatWidgetClass);
		BossStatWidget->AddToViewport(2); // 중간에 배치
		BossStatWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 플레이어 스텟 표시용 위젯 생성
	if (PlayerStatWidgetClass)
	{
		PlayerStatWidget = CreateWidget<UIVSimpleStatWidget>(GetWorld(), PlayerStatWidgetClass);
		PlayerStatWidget->AddToViewport(1); // 뒤에 배치
		PlayerStatWidget->SetVisibility(ESlateVisibility::Visible);
	}

	// 타겟팅 마크 위젯 생성
	if (TargetMarkerWidgetClass)
	{
		TargetMarkerWidget = CreateWidget<UUserWidget>(GetWorld(), TargetMarkerWidgetClass);
		TargetMarkerWidget->AddToViewport(0); // 가장 뒤에 배치
		TargetMarkerWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
	{
		UIVDeathEventSubsystem* LifeEventSubsystem = GameInstance->GetSubsystem<UIVDeathEventSubsystem>();
		if (LifeEventSubsystem)
		{
			// 사망 및 부활 이벤트 서브시스템 등록
			LifeEventSubsystem->PlayerDeathEventDelegate.AddUObject(this, &AIVSimpleStatHUD::OnPlayerDeath);
			LifeEventSubsystem->PlayerRespawnEventDelegate.AddUObject(this, &AIVSimpleStatHUD::OnPlayerAlive);
			LifeEventSubsystem->PlayerRespawnCompleteDelegate.AddUObject(this, &AIVSimpleStatHUD::BindPlayerStatWidget);
			LifeEventSubsystem->MonsterDeathEventDelegate.AddUObject(this, &AIVSimpleStatHUD::OnBossDeath);
			LifeEventSubsystem->MonsterDeathEventDelegate.AddUObject(this, &AIVSimpleStatHUD::OnTargetDeath);
		}
	}
}

void AIVSimpleStatHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bShowTargetMarker && TargetActor && TargetMarkerWidget)
	{
		FVector2D ScreenLocation;
		if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayerController(), TargetActor->GetActorLocation(), ScreenLocation))
		{
			TargetMarkerWidget->SetPositionInViewport(ScreenLocation);
		}
	}
}

void AIVSimpleStatHUD::OnPlayerDeath()
{
	// 타겟 마커 숨기고
	HideTargetMarker();

	// 플레이어 스탯 위젯 숨기고
	if(PlayerStatWidget)
	{
		PlayerStatWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 사망 위젯 만들어서 띄우기
	if (DeathWidget)
	{
		DeathWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AIVSimpleStatHUD::OnPlayerAlive()
{
	// 플레이어 스탯 위젯 띄우고
	if (PlayerStatWidget)
	{
		PlayerStatWidget->SetVisibility(ESlateVisibility::Visible);
	}

	// 사망 위젯 제거하기
	if (DeathWidget)
	{
		DeathWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AIVSimpleStatHUD::BindPlayerStatWidget()
{
	// 스탯 정보 갱신용 플레이어의 스탯 컴포넌트 가져오기
	APawn* PlayerPawn = GetOwningPawn();
	if (PlayerPawn && Cast<IIIVCharacterComponentProvider>(PlayerPawn))
	{
		IIIVCharacterComponentProvider* Provider = Cast<IIIVCharacterComponentProvider>(PlayerPawn);
		CharacterStatComponent = Provider->GetCharacterStatComponent();
	}

	// 위젯 갱신 함수와 스탯 컴포넌트 바인딩
	if (CharacterStatComponent && PlayerStatWidget)
	{
		CharacterStatComponent->OnBaseStatChanged.BindUObject(PlayerStatWidget, &UIVSimpleStatWidget::UpdatePlayerStatBars);
	
		// 현재 스탯 가져와서 위젯에 적용
		PlayerStatWidget->UpdatePlayerStatBars(CharacterStatComponent->GetBaseStat());
	}
}

void AIVSimpleStatHUD::OnBossDeath(AActor* DeadMonster)
{
	if (BossEnemy && BossEnemy == DeadMonster)
	{
		HideBossStatWidget();

		// 보스 클리어 위젯 띄우기
		if (BossClearWidget)
		{
			BossClearWidget->SetVisibility(ESlateVisibility::Visible);

			// 5초 후에 숨기기
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
				{
					if (BossClearWidget)
					{
						BossClearWidget->SetVisibility(ESlateVisibility::Collapsed);
					}
				}), 3.0f, false);
		}
	}
}

void AIVSimpleStatHUD::ShowBossStatWidget(AIVBossEnemy* Boss)
{
	BossEnemy = Boss;
	if (BossStatWidget && Boss)
	{
		// 이름 업데이트
		BossStatWidget->UpdateBossName(Boss->GetMonsterName());

		// 체력 바인딩 및 업데이트 
		IIIVMonsterComponentProvider* Provider = Cast<IIIVMonsterComponentProvider>(Boss);
		BossStatComponent = Provider->GetMonsterStatComponent();

		if (BossStatComponent)
		{
			BossStatComponent->OnHpChanged.BindUObject(BossStatWidget, &UIVSimpleBossStatWidget::UpdateBossHpBar);
			BossStatWidget->UpdateBossHpBar(BossStatComponent->GetMonsterMaxHp(), BossStatComponent->GetMonsterCurrentHp());
		}

		// 위젯 보이기
		BossStatWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AIVSimpleStatHUD::HideBossStatWidget()
{
	// 체력 바인딩 해제
	if (BossStatComponent)
	{
		BossStatComponent->OnHpChanged.Unbind();
	}
	BossStatComponent = nullptr;
	BossEnemy = nullptr;

	// 위젯 숨기기
	if (BossStatWidget)
	{
		BossStatWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AIVSimpleStatHUD::OnTargetDeath(AActor* DeadTarget)
{
	if (TargetActor && TargetActor == DeadTarget)
	{
		HideTargetMarker();
	}
}

void AIVSimpleStatHUD::ShowTargetMarker(AActor* Target)
{
	TargetActor = Target;
	bShowTargetMarker = true;
	
	if (TargetMarkerWidget)
	{
		TargetMarkerWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AIVSimpleStatHUD::HideTargetMarker()
{
	TargetActor = nullptr;
	bShowTargetMarker = false;

	if (TargetMarkerWidget)
	{
		TargetMarkerWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
