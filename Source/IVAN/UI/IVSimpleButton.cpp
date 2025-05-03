// Fill out your copyright notice in the Description page of Project Settings.


#include "IVSimpleButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UIVSimpleButton::NativeConstruct()
{
	// 버튼 클릭 이벤트 바인딩
	if (ChoiceButton)
	{
		ChoiceButton->OnClicked.AddDynamic(this, &UIVSimpleButton::OnChoiceButtonClicked);
	}
}

void UIVSimpleButton::InitButton(const FText& ButtonText, const FName& DialogueID)
{
	if (ChoiceText)
	{
		ChoiceText->SetText(ButtonText);
	}
	NextDialogueID = DialogueID;
}

void UIVSimpleButton::OnChoiceButtonClicked()
{
	OnButtonChoicedEvent.ExecuteIfBound(NextDialogueID);
}
