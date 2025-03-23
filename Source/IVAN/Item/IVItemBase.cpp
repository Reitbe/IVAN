// Fill out your copyright notice in the Description page of Project Settings.


#include "IVItemBase.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "IVAN/Item/IVInventoryComponent.h"
#include "Blueprint/UserWidget.h"

AIVItemBase::AIVItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// 플레이어가 상호작용 범위에 들어왔는지 확인하기 위한 콜리전의 초기 세팅
	InteractionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollision"));
	InteractionCollision->SetupAttachment(RootComponent);
	InteractionCollision->SetSphereRadius(100.0f);
	InteractionCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &AIVItemBase::OnInteractionSphereBeginOverlap);
	InteractionCollision->OnComponentEndOverlap.AddDynamic(this, &AIVItemBase::OnInteractionSphereEndOverlap);

	// 아이템 메쉬 초기 세팅
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMeshComponent->SetupAttachment(RootComponent);
	ItemMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	// 아이템 줍기 위젯 초기 세팅
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);

	// 출력할 위젯 클래스 지정
	static ConstructorHelpers::FClassFinder<UUserWidget> PickUpWidgetFinder
	(TEXT("/Game/Widget/WBP_ItemPickup.WBP_ItemPickup_C"));
	if (PickUpWidgetFinder.Class)
	{
		WidgetComponent->SetWidgetClass(PickUpWidgetFinder.Class);
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	}

	bIsInteractable = true;
}

void AIVItemBase::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetComponent)
	{
		PickUpWidget = Cast<UUserWidget>(WidgetComponent->GetUserWidgetObject());
		PickUpWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AIVItemBase::OnInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 정보 위젯 띄우기
	if (OtherActor->ActorHasTag("Player"))
	{
		ShowInteractionUI();
	}
}

void AIVItemBase::OnInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 정보 위젯 숨기기
	if (OtherActor->ActorHasTag("Player"))
	{
		HideInteractionUI();
	}
}

void AIVItemBase::InitializeItem(const FItemBaseInfo& InItemInfo)
{
	ItemInfo = InItemInfo;
	ItemMeshComponent->SetStaticMesh(ItemInfo.ItemMesh);
}

void AIVItemBase::DropItem()
{
	/* 아이템 데이터베이스에서 그 액터 레퍼런스 찾은다음 그걸로 생성, 데이터 채워넣기, 아래 설정하기*/

	//ItemMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	//ItemMeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	//ItemMeshComponent->SetSimulatePhysics(true);
}

void AIVItemBase::Interact(AActor* InteractingActor)
{
	if (!bIsInteractable || !InteractingActor) return;

	UIVInventoryComponent* InventoryComponent = InteractingActor->FindComponentByClass<UIVInventoryComponent>();
	if (InventoryComponent)
	{
		// 인벤토리에 추가 성공 시 레벨에서 아이템 제거
		if (InventoryComponent->AddItemToInventory(ItemInfo.ItemID))
		{
			HideInteractionUI();
			SetInteractable(false);
			Destroy();
		}
	}
}

void AIVItemBase::SetInteractable(bool bNewInteractable)
{
	bIsInteractable = bNewInteractable;
}

void AIVItemBase::ShowInteractionUI()
{
	if (bIsInteractable && PickUpWidget)
	{
		PickUpWidget->SetVisibility(ESlateVisibility::Visible);
	}	
}

void AIVItemBase::HideInteractionUI()
{
	if (bIsInteractable && PickUpWidget)
	{
		PickUpWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AIVItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

