// Fill out your copyright notice in the Description page of Project Settings.


#include "IVDialogueWidget.h"
#include "IVAN/UI/IVSimpleButton.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"


UIVDialogueWidget::UIVDialogueWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 선택지를 담을 버튼 클래스
	static ConstructorHelpers::FClassFinder<UIVSimpleButton> ChoiceButtonClassFinder
	(TEXT("/Game/Widget/WBP_ChoiceButton.WBP_ChoiceButton_C"));
	if (ChoiceButtonClassFinder.Class)
	{
		ChoiceButtonClass = ChoiceButtonClassFinder.Class;
	}
}

void UIVDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// '다음' 버튼 바인딩. 다음 대화 ID를 전달한다.
	if (NextDialogueButton)
	{
		NextDialogueButton->OnButtonChoicedEvent.BindDynamic(this, &UIVDialogueWidget::OnChoiceButtonClicked);
	}
}

void UIVDialogueWidget::UpdateDialogue(const FDialogueInfo& CurrentDialogue)
{
	// 대화 내용 업데이트
	DialogueSpeakerTextBlock->SetText(CurrentDialogue.SpeakerName);
	DialogueContentTextBlock->SetText(CurrentDialogue.DialogueText);

	// 기존 버튼 초기화
	ChoiceButtonBox->ClearChildren();
	NextDialogueButton->SetVisibility(ESlateVisibility::Collapsed);

	
	int32 ChoiceCount = CurrentDialogue.DialogueOptions.Num();
	if (ChoiceCount <= 1) // 단일 선택지 = 다음 버튼
	{
		NextDialogueButton->SetVisibility(ESlateVisibility::Visible);
		FText NextButtonText = FText::FromString(TEXT("다음"));
		NextDialogueButton->InitButton(NextButtonText, CurrentDialogue.DialogueOptions[0].NextDialogueID);
	}
	else // 선택지의 수를 기반으로 버튼 생성
	{
		for (int32 i = 0; i < ChoiceCount; ++i)
		{
			UIVSimpleButton* ChoiceButton = CreateWidget<UIVSimpleButton>(GetWorld(), ChoiceButtonClass);
			if (ChoiceButton)
			{
				ChoiceButton->InitButton(CurrentDialogue.DialogueOptions[i].OptionText, CurrentDialogue.DialogueOptions[i].NextDialogueID);
				ChoiceButton->OnButtonChoicedEvent.BindDynamic(this, &UIVDialogueWidget::OnChoiceButtonClicked);
				ChoiceButtonBox->AddChildToVerticalBox(ChoiceButton);
			}
		}
	}
}

void UIVDialogueWidget::OnChoiceButtonClicked(const FName& DialogueID)
{
	OnButtonChoicedEvent.ExecuteIfBound(DialogueID);
}
