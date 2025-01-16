// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IVAN/IVGenericStructs.h"
#include "IVSimpleStatWidget.generated.h"

class UIVBaseStatBar;

/**
 * 플레이어 스탯 중 HP, Stamina를 표시하는 위젯
 */

UCLASS()
class IVAN_API UIVSimpleStatWidget : public UUserWidget
{
	GENERATED_BODY()

// 기본
public:
	virtual void NativeConstruct() override;

// 플레이어 스탯 바
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UIVBaseStatBar> PlayerHpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UIVBaseStatBar> PlayerStaminaBar;

public:
	/* 플레이어 스탯이 변경될 시, 이를 위젯에 반영 */
	void UpdatePlayerStatBars(const FBaseStat& BaseStat);
	void UpdatePlayerHpBar(float NewMaxHP, float NewCurrentHP) const;
	void UpdatePlayerStaminaBar(float NewMaxStamina, float NewCurrentStamina) const;

};
