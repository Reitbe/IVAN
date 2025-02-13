// Fill out your copyright notice in the Description page of Project Settings.


#include "IVSimpleBossStatWidget.h"
#include "IVAN/UI/IVBaseStatBar.h"
#include "Components/TextBlock.h"

void UIVSimpleBossStatWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UIVSimpleBossStatWidget::UpdateBossName(const FName& BossName) const
{
	BossNameBlock->SetText(FText::FromName(BossName));
}

void UIVSimpleBossStatWidget::UpdateBossHpBar(float NewMaxHP, float NewCurrentHP) const
{
	BossHpBar->UpdateStatBar(NewMaxHP, NewCurrentHP);
}
