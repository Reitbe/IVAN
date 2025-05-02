// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IVAN/IVGenericStructs.h"
#include "IVDialogueWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDialogueChoiced, const FName&, DialogueID);

class UTextBlock;
class UVerticalBox;
class UIVSimpleButton;

/**
 * 대화와 관련된 출력을 담당하는 위젯. 
 * 발화자, 대화 내용, 선택지를 표시한다.
 */
UCLASS()
class IVAN_API UIVDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UIVDialogueWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;


// 기능
public:
	/* 대화 내용을 업데이트하는 함수 */
	void UpdateDialogue(const FDialogueInfo& CurrentDialogue);

	/* 대화 선택지 버튼을 클릭했을 때, 그 결과를 받아 HUD에 전달하는 함수 */
	UFUNCTION()
	void OnChoiceButtonClicked(const FName& DialogueID);

	/* HUD에 전달할 때 사용할 대리자 */
	FOnDialogueChoiced OnButtonChoicedEvent;

	
// 선택 버튼
protected:
	/* 선택지를 담을 버튼 클래스*/
	TSubclassOf<UIVSimpleButton> ChoiceButtonClass;


// 위젯 구성 요소
public: 
	/* 누가 말하고 있는가*/
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogueSpeakerTextBlock;

	/* 대화 내용 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogueContentTextBlock;

	/* 선택지를 담을 수직 박스 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> ChoiceButtonBox;

	/* 다음 대화로 넘어가기 위한 버튼 - 선택지가 하나밖에 없을 때*/
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UIVSimpleButton> NextDialogueButton;
	
};
