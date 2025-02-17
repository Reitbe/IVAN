// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "IVSimpleStatHUD.generated.h"

class UIVSimpleStatWidget;
class UIVSimpleBossStatWidget;
class UIVCharacterStatComponent;
class UIVMonsterStatComponent;
class AIVBossEnemy;

/**
 * 플레이어에게 필요한 HUD 정보 관리
 * 사망, 플레이어 스탯, 보스몬스터 스탯과 관련된 UI를 관리한다.
 */

UCLASS()
class IVAN_API AIVSimpleStatHUD : public AHUD
{
	GENERATED_BODY()

// 기본
public:
	AIVSimpleStatHUD();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;


// 플레이어 사망 및 부활 처리
protected:
	void OnPlayerDeath();
	void OnPlayerAlive();
	void BindPlayerStatWidget();

// 보스 스탯 표시
public:
	void OnBossDeath(AActor* DeadMonster);
	void ShowBossStatWidget(AIVBossEnemy* Boss);
	void HideBossStatWidget();

// 타겟팅 마커 표시
	void OnTargetDeath(AActor* DeadTarget);
	void ShowTargetMarker(AActor* Target);
	void HideTargetMarker();

// UI 관리
protected:
	/* 캐릭터 스텟 위젯 갱신을 위한 정보 습득용 컴포넌트 */
	TObjectPtr<UIVCharacterStatComponent> CharacterStatComponent;

	/* 보스 스텟 위젯 갱신을 위한 정보 습득용 컴포넌트 */ 
	TObjectPtr<UIVMonsterStatComponent> BossStatComponent;
	TObjectPtr<AActor> BossEnemy;

	/* 타겟팅 마커 위젯 갱신용 */
	TObjectPtr<AActor> TargetActor;
	uint8 bShowTargetMarker : 1;


// UI 
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UIVSimpleStatWidget> PlayerStatWidgetClass;
	TObjectPtr< UIVSimpleStatWidget> PlayerStatWidget;
	
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UUserWidget> DeathWidgetClass;
	TObjectPtr<UUserWidget> DeathWidget;

	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UIVSimpleBossStatWidget> BossStatWidgetClass;
	TObjectPtr<UIVSimpleBossStatWidget> BossStatWidget;

	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UUserWidget> BossClearWidgetClass;
	TObjectPtr<UUserWidget> BossClearWidget;

	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UUserWidget> TargetMarkerWidgetClass;
	TObjectPtr<UUserWidget> TargetMarkerWidget;


// 위젯 애니메이션
//public:
//	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "UI", meta = (BindWidgetAnim))
//	TObjectPtr<UWidgetAnimation> FadeWidgetAnimation;


};
