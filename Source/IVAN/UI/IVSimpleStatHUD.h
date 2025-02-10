// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "IVSimpleStatHUD.generated.h"

class UIVSimpleStatWidget;
class UIVCharacterStatComponent;

/**
 * 플레이어 기본 스텟의 HUD 표시 관리
 */

UCLASS()
class IVAN_API AIVSimpleStatHUD : public AHUD
{
	GENERATED_BODY()

public:
	AIVSimpleStatHUD();
	virtual void BeginPlay() override;

// 사망 처리
protected:
	void OnPlayerDeath();
	void OnPlayerAlive();

// UI 관리
protected:
	/* 스텟 위젯 갱신을 위한 정보 습득용 컴포넌트 */
	TObjectPtr<UIVCharacterStatComponent> CharacterStatComponent;

// UI 
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UIVSimpleStatWidget> SimpleStatWidgetClass;
	TObjectPtr< UIVSimpleStatWidget> SimpleStatWidget;
	
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UUserWidget> DeathWidgetClass;
	TObjectPtr<UUserWidget> DeathWidget;
};
