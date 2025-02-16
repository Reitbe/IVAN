// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IVPlayerDeathWidget.generated.h"

class UButton;

/**
 * 플레이어 사망 시 전체 화면에 표시되는 YOU DIED 위젯
 * 화면 아무 곳이나 클릭해도 부활 이벤트로 연결된다.
 */
UCLASS()
class IVAN_API UIVPlayerDeathWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnRespawnButtonClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RespawnButton;
};
