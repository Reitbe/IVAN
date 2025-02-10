// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IVPlayerDeathWidget.generated.h"

class UButton;

/**
 * 
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
