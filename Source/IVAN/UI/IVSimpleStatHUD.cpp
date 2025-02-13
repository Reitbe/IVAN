// Fill out your copyright notice in the Description page of Project Settings.


#include "IVSimpleStatHUD.h"
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

	// 사망 시 표시할 위젯 클래스
	static ConstructorHelpers::FClassFinder<UUserWidget> BossStatWidgetClassFinder
	(TEXT("/Game/Widget/WBP_SimpleBossStat.WBP_SimpleBossStat_C"));
	if (BossStatWidgetClassFinder.Class)
	{
		BossStatWidgetClass = BossStatWidgetClassFinder.Class;
	}
}

void AIVSimpleStatHUD::BeginPlay()
{
	Super::BeginPlay();

	// 사망 시 표시할 위젯 생성
	if (DeathWidgetClass)
	{
		DeathWidget = CreateWidget<UUserWidget>(GetWorld(), DeathWidgetClass);
		DeathWidget->AddToViewport(2); // 앞에 배치
		DeathWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 플레이어 스텟 표시용 위젯 생성
	if (PlayerStatWidgetClass)
	{
		PlayerStatWidget = CreateWidget<UIVSimpleStatWidget>(GetWorld(), PlayerStatWidgetClass);
		PlayerStatWidget->AddToViewport(0); // 뒤에 배치
		PlayerStatWidget->SetVisibility(ESlateVisibility::Visible);
	}

	// 보스 스텟 표시용 위젯 생성
	if (BossStatWidgetClass)
	{
		BossStatWidget = CreateWidget<UIVSimpleBossStatWidget>(GetWorld(), BossStatWidgetClass);
		BossStatWidget->AddToViewport(1); // 중간에 배치
		BossStatWidget->SetVisibility(ESlateVisibility::Collapsed);
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
		}
	}
}

void AIVSimpleStatHUD::OnPlayerDeath()
{
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
