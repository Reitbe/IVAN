// Fill out your copyright notice in the Description page of Project Settings.


#include "IVSimpleStatWidget.h"
#include "IVAN/UI/IVBaseStatBar.h"

void UIVSimpleStatWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UIVSimpleStatWidget::UpdatePlayerStatBars(const FBaseStat& BaseStat)
{
	float MaxHP = BaseStat.MaxHP;
	float CurrentHP = BaseStat.CurrentHP;
	UpdatePlayerHpBar(MaxHP, CurrentHP);

	float MaxStamina = BaseStat.MaxStamina;
	float CurrentStamina = BaseStat.CurrentStamina;
	UpdatePlayerStaminaBar(MaxStamina, CurrentStamina);
}

void UIVSimpleStatWidget::UpdatePlayerHpBar(float NewMaxHP, float NewCurrentHP) const
{
	PlayerHpBar->UpdateStatBar(NewMaxHP, NewCurrentHP);
}

void UIVSimpleStatWidget::UpdatePlayerStaminaBar(float NewMaxStamina, float NewCurrentStamina) const
{
	PlayerStaminaBar->UpdateStatBar(NewMaxStamina, NewCurrentStamina);
}
