// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "IVAN/IVGenericStructs.h"
#include "IVSimpleStatHUD.generated.h"

DECLARE_DELEGATE_OneParam(FContinueDialogue, const FName& /*NextDialogueID*/);

class UIVSimpleStatWidget;
class UIVSimpleBossStatWidget;
class UIVInventoryBaseWidget;
class UIVQuickSlotWidget;
class UIVDialogueWidget;
class UIVCharacterStatComponent;
class UIVMonsterStatComponent;
class AIVBossEnemy;
class USoundCue;
class UAudioComponent;

/**
 * 플레이어에게 필요한 HUD 정보 관리
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


// 인벤토리 표시
	/* 플레이어 캐릭터 빙의 이후 진행되는 인벤토리 컴포넌트-위젯 연결*/
	void BindInventoryWidget();
	void ShowInventory();
	void HideInventory();


// 메뉴 표시
	void ShowMenu();
	void HideMenu();

// 대화 UI
	/* 대화 매니저에게 다음 대화 ID를 전달하기 위한 대리자 */
	FContinueDialogue ContinueDialogueDelegate;

	/* 대화 모드 활성화 여부 */
	UFUNCTION()
	void SetDialogueMode(bool bDialogueMode);

	/* 대화 UI 업데이트 */
	void UpdateDialogue(const FDialogueInfo& CurrentDialogue);

	/* 대화 UI에서 선택 결과를 제공받았으므로 대화 매니저에 이를 전달 */
	UFUNCTION()
	void OnChiceNextDialogue(const FName& NextDialogueID);

protected:
	/* 대화 모드 여부 */
	bool bOnDialogueMode;


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
	/* 플레이어 스탯 */
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UIVSimpleStatWidget> PlayerStatWidgetClass;
	TObjectPtr< UIVSimpleStatWidget> PlayerStatWidget;
	
	/* 사망 위젯 */
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UUserWidget> DeathWidgetClass;
	TObjectPtr<UUserWidget> DeathWidget;

	/* 보스 스탯 위젯 */
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UIVSimpleBossStatWidget> BossStatWidgetClass;
	TObjectPtr<UIVSimpleBossStatWidget> BossStatWidget;

	/* 보스 클리어 위젯 */
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UUserWidget> BossClearWidgetClass;
	TObjectPtr<UUserWidget> BossClearWidget;

	/* 타겟 마커 위젯 */
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UUserWidget> TargetMarkerWidgetClass;
	TObjectPtr<UUserWidget> TargetMarkerWidget;

	/* 인벤토리 위젯 */
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UIVInventoryBaseWidget> InventoryBaseWidgetClass;
	TObjectPtr<UIVInventoryBaseWidget> InventoryBaseWidget;

	/* 메뉴 위젯 */
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;
	TObjectPtr<UUserWidget> MenuWidget;

	/* 퀵슬롯 위젯 */ 
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UIVQuickSlotWidget> QuickSlotWidgetClass;
	TObjectPtr<UIVQuickSlotWidget> QuickSlotWidget;

	/* 대화 위젯 */
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UIVDialogueWidget> DialogueWidgetClass;
	TObjectPtr<UIVDialogueWidget> DialogueWidget;


// 사운드
protected:
	/* 사망 위젯 등장 사운드 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<USoundCue> PlayerDeathWidgetSound;

	/* 보스 클리어 위젯 등장 사운드 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<USoundCue> BossClearWidgetSound;

	/* 부활시 사망 사운드 중단을 위한 오디오 컴포넌트*/
	TObjectPtr<UAudioComponent> PlayerDeathAudioComponent;

};
