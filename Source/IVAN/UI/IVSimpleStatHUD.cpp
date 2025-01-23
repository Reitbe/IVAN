// Fill out your copyright notice in the Description page of Project Settings.


#include "IVSimpleStatHUD.h"
#include "IVAN/UI/IVBaseStatBar.h"
#include "IVAN/UI/IVSimpleStatWidget.h"
#include "IVAN/Interface/IIVCharacterComponentProvider.h"
#include "IVAN/Stat/IVCharacterStatComponent.h"
#include "IVAN/GameSystem/IVDeathEventSubsystem.h"

AIVSimpleStatHUD::AIVSimpleStatHUD()
{
	// HUD에 표시할 위젯 클래스
	static ConstructorHelpers::FClassFinder<UIVSimpleStatWidget> SimpleStatWidgetClassFinder
	(TEXT("/Game/Widget/WBP_SimpleStat.WBP_SimpleStat_C"));
	if (SimpleStatWidgetClassFinder.Class)
	{
		SimpleStatWidgetClass = SimpleStatWidgetClassFinder.Class;
	}
}

void AIVSimpleStatHUD::BeginPlay()
{
	Super::BeginPlay();

	// 스탯 정보 갱신용 플레이어의 스탯 컴포넌트 가져오기
	APawn* PlayerPawn = GetOwningPawn();
	if (PlayerPawn && Cast<IIIVCharacterComponentProvider>(PlayerPawn))
	{
		IIIVCharacterComponentProvider* Provider = Cast<IIIVCharacterComponentProvider>(PlayerPawn);
		CharacterStatComponent = Provider->GetCharacterStatComponent();
	}

	// 플레이어 스텟 표시용 위젯 생성
	if (SimpleStatWidgetClass)
	{
		SimpleStatWidget = CreateWidget<UIVSimpleStatWidget>(GetWorld(), SimpleStatWidgetClass);
		SimpleStatWidget->AddToViewport();
		SimpleStatWidget->SetVisibility(ESlateVisibility::Visible);
	}

	// 위젯 갱신 함수와 스탯 컴포넌트 바인딩
	if (CharacterStatComponent && SimpleStatWidget)
	{
		CharacterStatComponent->OnBaseStatChanged.BindUObject(SimpleStatWidget, &UIVSimpleStatWidget::UpdatePlayerStatBars);
	}

	// 사망 이벤트 서브시스템 등록
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
	{
		UIVDeathEventSubsystem* LifeEventSubsystem = GameInstance->GetSubsystem<UIVDeathEventSubsystem>();
		if (LifeEventSubsystem)
		{
			LifeEventSubsystem->PlayerDeathEventDelegate.AddUObject(this, &AIVSimpleStatHUD::OnPlayerDeath);
			LifeEventSubsystem->PlayerRespawnEventDelegate.AddUObject(this, &AIVSimpleStatHUD::OnPlayerAlive);
		}
	}

}

void AIVSimpleStatHUD::OnPlayerDeath()
{

	if(SimpleStatWidget)
	{
		SimpleStatWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AIVSimpleStatHUD::OnPlayerAlive()
{
	if (SimpleStatWidget)
	{
		SimpleStatWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
