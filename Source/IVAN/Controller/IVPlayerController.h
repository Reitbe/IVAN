// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IVPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class AIVSimpleStatHUD;

/**
 * 본 게임에서 사용되는 플레이어 컨트롤러.
 * 입력에 따른 캐릭터 조종 작업중.
 */

UCLASS()
class IVAN_API AIVPlayerController : public APlayerController
{
	GENERATED_BODY()

// 기본
public:
	AIVPlayerController();

protected:
	virtual void BeginPlay() override;

// HUD
protected:
	UPROPERTY(EditAnywhere, Category = "HUD")
	TObjectPtr<AIVSimpleStatHUD> SimpleStatHUD;

// 입력
public:
	virtual void SetupInputComponent() override;

private:
	void InputConstructHelper();

protected:
	/* 멀티 메뉴 표시 */
	void ShowMenu();

	/* 입력 컨텍스트 및 액션 */
	TObjectPtr<UInputMappingContext> InputMappingContext;
	TObjectPtr<UInputAction> ShowMenuAction;
};
