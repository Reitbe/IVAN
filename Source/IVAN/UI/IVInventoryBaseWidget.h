// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IVInventoryBaseWidget.generated.h"

class UIVInventoryWidget;

/**
 * 인벤토리 위젯이 움직일 수 있는 바탕 캔버스 위젯
 */
UCLASS()
class IVAN_API UIVInventoryBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	/* 인벤토리 창 위젯 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UIVInventoryWidget> InventoryWidget;
};
