// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IVSimpleBossStatWidget.generated.h"

class UIVBaseStatBar;
class UTextBlock;

/**
 * 보스몬스터의 이름 및 체력을 표시하는 위젯
 */
UCLASS()
class IVAN_API UIVSimpleBossStatWidget : public UUserWidget
{
	GENERATED_BODY()

// 기본
public:
	virtual void NativeConstruct() override;

// 몬스터 스탯 바
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UIVBaseStatBar> BossHpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> BossNameBlock;
	

// 업데이트 함수
public:
	/* 보스몬스터의 이름 업데이트 */
	void UpdateBossName(const FName& BossName) const;

	/* 보스몬스터의 체력바 업데이트 */
	void UpdateBossHpBar(float NewMaxHP, float NewCurrentHP) const;
};
