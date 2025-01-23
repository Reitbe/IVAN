// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IVBaseStatBar.generated.h"

class UProgressBar;

/**
 * 스탯 바의 기본 구성 요소만을 담은 클래스
 */
UCLASS()
class IVAN_API UIVBaseStatBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	/* 스탯 변동 사항 발생 시 위젯 업데이트*/
	void UpdateStatBar(float NewMaxStat, float NewCurrentStat);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StatBar;

private:
	float MaxStat;
	float CurrentStat;
	
};
