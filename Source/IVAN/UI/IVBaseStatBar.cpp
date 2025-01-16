// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBaseStatBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UIVBaseStatBar::NativeConstruct()
{
	Super::NativeConstruct();

	MaxStat = 100.0f;
	CurrentStat = 100.0f;
	UpdateStatBar(MaxStat, CurrentStat);
}

void UIVBaseStatBar::UpdateStatBar(float NewMaxStat, float NewCurrentStat)
{
	MaxStat = NewMaxStat;
	CurrentStat = NewCurrentStat;

	if (StatBar)
	{
		float Percent = CurrentStat / MaxStat;
		StatBar->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
	}
}
