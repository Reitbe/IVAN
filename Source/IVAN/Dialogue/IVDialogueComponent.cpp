
#include "IVDialogueComponent.h"
#include "IVAN/GameSystem/IVDialogueManagerSubsystem.h"

UIVDialogueComponent::UIVDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 기본 NPC ID는 None. NPC마다 별도 지정 필요.
	NPCID = NAME_None;
}


void UIVDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UIVDialogueComponent::StartDialogue()
{
	// 대화 서브시스템에 NPC ID를 제공하며 대화를 시작한다.
	UIVDialogueManagerSubsystem* DialogueSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UIVDialogueManagerSubsystem>();
	if (DialogueSubsystem)
	{
		DialogueSubsystem->StartDialogue(NPCID);
	}
}

