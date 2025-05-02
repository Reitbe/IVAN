// Fill out your copyright notice in the Description page of Project Settings.


#include "IVNPC.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "IVAN/Dialogue/IVDialogueComponent.h"
#include "Blueprint/UserWidget.h"

AIVNPC::AIVNPC()
{
	// 대화 컴포넌트 
	DialogueComponent = CreateDefaultSubobject<UIVDialogueComponent>(TEXT("DialogueComponent"));

	// 플레이어가 상호작용 범위에 들어왔는지 확인하기 위한 콜리전의 초기 세팅
	InteractionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollision"));
	InteractionCollision->SetupAttachment(RootComponent);
	InteractionCollision->SetSphereRadius(100.0f);
	InteractionCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &AIVNPC::OnInteractionSphereBeginOverlap);
	InteractionCollision->OnComponentEndOverlap.AddDynamic(this, &AIVNPC::OnInteractionSphereEndOverlap);

	// 상호작용 위젯 세팅
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);

	// 출력할 위젯 클래스 지정
	static ConstructorHelpers::FClassFinder<UUserWidget> TalkInteractionWidgetFinder
	(TEXT("/Game/Widget/WBP_TalkInteraction.WBP_TalkInteraction_C"));
	if (TalkInteractionWidgetFinder.Class)
	{
		WidgetComponent->SetWidgetClass(TalkInteractionWidgetFinder.Class);
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	}

	bIsInteractable = true;
	bIsInDialogue = false;
}

void AIVNPC::BeginPlay()
{
	Super::BeginPlay();

	// 상호작용 위젯 숨기기
	if (WidgetComponent)
	{
		TalkInteractionWidget = Cast<UUserWidget>(WidgetComponent->GetUserWidgetObject());
		TalkInteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AIVNPC::OnInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player") && bIsInteractable)
	{
		ShowInteractionUI();
	}
}

void AIVNPC::OnInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player") && bIsInteractable)
	{
		if (bIsInDialogue)
		{
			bIsInDialogue = false;
		}
		HideInteractionUI();
	}
}

void AIVNPC::Interact(AActor* InteractingActor)
{
	if (!bIsInDialogue) // 기존에 대화중이지 않은 경우에만 대화 시작 가능
	{
		bIsInDialogue = true;
		HideInteractionUI();
		DialogueComponent->StartDialogue();
	}
}

void AIVNPC::SetInteractable(bool bNewInteractable)
{
	bIsInteractable = bNewInteractable;
}

void AIVNPC::ShowInteractionUI()
{
	if (bIsInteractable && TalkInteractionWidget)
	{
		TalkInteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AIVNPC::HideInteractionUI()
{
	if (bIsInteractable && TalkInteractionWidget)
	{
		TalkInteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
