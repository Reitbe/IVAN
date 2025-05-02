// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IVSimpleButton.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnButtonChoiced, const FName&, DialogueID);

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class IVAN_API UIVSimpleButton : public UUserWidget
{
	GENERATED_BODY()

// 초기화
public:
	virtual void NativeConstruct() override;
	void InitButton(const FText& ButtonText, const FName& DialogueID);


// 위젯 요소
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ChoiceButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ChoiceText;


// 버튼 클릭 이벤트
public:
	/* 버튼 클릭 시 호출되는 함수 */
	UFUNCTION()
	void OnChoiceButtonClicked();

	/* 버튼이 눌렸을 때 전달할 이벤트 */
	FOnButtonChoiced OnButtonChoicedEvent;
	
	/* 버튼에 할당된 다음 대화 ID */
	FName NextDialogueID;
	
};
