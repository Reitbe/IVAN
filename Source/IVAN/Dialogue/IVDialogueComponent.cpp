
#include "IVDialogueComponent.h"
#include "IVAN/GameSystem/IVDialogueManagerSubsystem.h"

UIVDialogueComponent::UIVDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 기본 NPC ID는 None
	NPCID = NAME_None;
}


void UIVDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UIVDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UIVDialogueComponent::StartDialogue()
{
	// 대화 서브시스템에 접근하여 ID를 제공하며 대화 시작
	UIVDialogueManagerSubsystem* DialogueSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UIVDialogueManagerSubsystem>();
	if (DialogueSubsystem)
	{
		DialogueSubsystem->StartDialogue(NPCID);
	}
}

