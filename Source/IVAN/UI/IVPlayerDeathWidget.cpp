// Fill out your copyright notice in the Description page of Project Settings.


#include "IVPlayerDeathWidget.h"
#include "Components/Button.h"
#include "IVAN/GameSystem/IVDeathEventSubsystem.h"

void UIVPlayerDeathWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RespawnButton)
	{
		RespawnButton->OnClicked.AddDynamic(this, &UIVPlayerDeathWidget::OnRespawnButtonClicked);
	}
}

void UIVPlayerDeathWidget::OnRespawnButtonClicked()
{
	// 글로벌 서브시스템을 통해 플레이어 부활 이벤트를 브로드캐스트
	UIVDeathEventSubsystem* DeathEventSubsystem = GetGameInstance()->GetSubsystem<UIVDeathEventSubsystem>();
	if (DeathEventSubsystem)
	{
		DeathEventSubsystem->PlayerRespawnEventDelegate.Broadcast();
	}
}
