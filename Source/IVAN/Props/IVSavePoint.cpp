// Fill out your copyright notice in the Description page of Project Settings.


#include "IVSavePoint.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "IVAN/GameSystem/IVSaveManagerSubsystem.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AIVSavePoint::AIVSavePoint()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComponent;

	// 메쉬 초기 세팅
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	// 플레이어가 상호작용 범위에 들어왔는지 확인하기 위한 콜리전의 초기 세팅
	InteractionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollision"));
	InteractionCollision->SetupAttachment(RootComponent);
	InteractionCollision->SetSphereRadius(100.0f);
	InteractionCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &AIVSavePoint::OnInteractionSphereBeginOverlap);
	InteractionCollision->OnComponentEndOverlap.AddDynamic(this, &AIVSavePoint::OnInteractionSphereEndOverlap);

	// 상호작용 위젯 초기 세팅
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);

	// 출력할 위젯 클래스 지정
	static ConstructorHelpers::FClassFinder<UUserWidget> InteractionWidgetFinder
	(TEXT("/Game/Widget/WBP_Interaction.WBP_Interaction_C"));
	if (InteractionWidgetFinder.Class)
	{
		WidgetComponent->SetWidgetClass(InteractionWidgetFinder.Class);
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	}

	bIsInteractable = true;
}

void AIVSavePoint::BeginPlay()
{
	Super::BeginPlay();

	// 상호작용 위젯 숨기기
	if (WidgetComponent)
	{
		InteractionWidget = Cast<UUserWidget>(WidgetComponent->GetUserWidgetObject());
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AIVSavePoint::OnInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 정보 위젯 띄우기
	if (OtherActor->ActorHasTag("Player"))
	{
		ShowInteractionUI();
	}
}

void AIVSavePoint::OnInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 정보 위젯 숨기기
	if (OtherActor->ActorHasTag("Player"))
	{
		HideInteractionUI();
	}
}

void AIVSavePoint::PlaySaveEffect(FTransform SaveLocation)
{
	if (SaveEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SaveEffect, SaveLocation);
	}
}

void AIVSavePoint::PlaySaveSound(FTransform SaveLocation)
{
	if (SaveSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SaveSound, SaveLocation.GetLocation());
	}
}

void AIVSavePoint::Interact(AActor* InteractingActor)
{
	if (!bIsInteractable || !InteractingActor) return;

	UIVSaveManagerSubsystem* SaveManagerSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UIVSaveManagerSubsystem>();
	if (SaveManagerSubsystem)
	{
		SaveManagerSubsystem->RequestSaveGame();
		SaveManagerSubsystem->SaveTransform(InteractingActor->GetActorTransform()); // 플레이어의 현재 위치 저장

		PlaySaveEffect(GetTransform());
		PlaySaveSound(GetTransform());
	}
}

void AIVSavePoint::SetInteractable(bool bNewInteractable)
{
	bIsInteractable = bNewInteractable;
}

void AIVSavePoint::ShowInteractionUI()
{
	if (bIsInteractable && InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AIVSavePoint::HideInteractionUI()
{
	if (bIsInteractable && InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

