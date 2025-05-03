// Fill out your copyright notice in the Description page of Project Settings.


#include "IVSimpleStatHUD.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStatics.h"
#include "IVAN/Enemy/IVBossEnemy.h"
#include "IVAN/Interface/IIVCharacterComponentProvider.h"
#include "IVAN/Interface/IIVMonsterComponentProvider.h"
#include "IVAN/Interface/IIVInventoryComponentProvider.h"
#include "IVAN/UI/IVBaseStatBar.h"
#include "IVAN/UI/IVSimpleStatWidget.h"
#include "IVAN/UI/IVSimpleBossStatWidget.h"
#include "IVAN/UI/IVInventoryBaseWidget.h"
#include "IVAN/UI/IVInventoryWidget.h"
#include "IVAN/UI/IVQuickSlotWidget.h"
#include "IVAN/UI/IVDialogueWidget.h"
#include "IVAN/Stat/IVCharacterStatComponent.h"
#include "IVAN/Stat/IVMonsterStatComponent.h"
#include "IVAN/Item/IVInventoryComponent.h"
#include "IVAN/GameSystem/IVDeathEventSubsystem.h"
#include "IVAN/GameSystem/IVDialogueManagerSubsystem.h"

AIVSimpleStatHUD::AIVSimpleStatHUD()
{
	PrimaryActorTick.bCanEverTick = true;

	// HUD에 표시할 위젯 클래스
	static ConstructorHelpers::FClassFinder<UIVSimpleStatWidget> PlayerStatWidgetClassFinder
	(TEXT("/Game/Widget/WBP_SimpleStat.WBP_SimpleStat_C"));
	if (PlayerStatWidgetClassFinder.Class)
	{
		PlayerStatWidgetClass = PlayerStatWidgetClassFinder.Class;
	}

	// 사망 시 표시할 위젯 클래스
	static ConstructorHelpers::FClassFinder<UUserWidget> DeathWidgetClassFinder
	(TEXT("/Game/Widget/WBP_YouDie.WBP_YouDie_C"));
	if (DeathWidgetClassFinder.Class)
	{
		DeathWidgetClass = DeathWidgetClassFinder.Class;
	}

	// 보스 스텟 표시용 위젯 클래스
	static ConstructorHelpers::FClassFinder<UUserWidget> BossStatWidgetClassFinder
	(TEXT("/Game/Widget/WBP_SimpleBossStat.WBP_SimpleBossStat_C"));
	if (BossStatWidgetClassFinder.Class)
	{
		BossStatWidgetClass = BossStatWidgetClassFinder.Class;
	}

	// 보스 클리어 위젯 클래스
	static ConstructorHelpers::FClassFinder<UUserWidget> BossClearWidgetClassFinder
	(TEXT("/Game/Widget/WBP_BossClear.WBP_BossClear_C"));
	if (BossClearWidgetClassFinder.Class)
	{
		BossClearWidgetClass = BossClearWidgetClassFinder.Class;
	}

	// 타겟팅 대상에 부착할 마커 클래스
	static ConstructorHelpers::FClassFinder<UUserWidget> TargetMarkerWidgetClassFinder
	(TEXT("/Game/Widget/WBP_TargetMarker.WBP_TargetMarker_C"));
	if (TargetMarkerWidgetClassFinder.Class)
	{
		TargetMarkerWidgetClass = TargetMarkerWidgetClassFinder.Class;
	}

	// 인벤토리 위젯 클래스
	static ConstructorHelpers::FClassFinder<UIVInventoryBaseWidget> InventoryWidgetClassFinder
	(TEXT("/Game/Widget/WBP_InventoryBase.WBP_InventoryBase_C"));
	if (InventoryWidgetClassFinder.Class)
	{
		InventoryBaseWidgetClass = InventoryWidgetClassFinder.Class;
	}

	// 메뉴 위젯 클래스
	//static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidgetClassFinder
	//(TEXT("/Game/Widget/WBP_Menu.WBP_Menu_C"));
	//if (MenuWidgetClassFinder.Class)
	//{
	//	MenuWidgetClass = MenuWidgetClassFinder.Class;
	//}

	// 퀵슬롯 위젯 클래스
	static ConstructorHelpers::FClassFinder<UIVQuickSlotWidget> QuickSlotWidgetClassFinder
	(TEXT("/Game/Widget/WBP_QuickSlot.WBP_QuickSlot_C"));
	if (QuickSlotWidgetClassFinder.Class)
	{
		QuickSlotWidgetClass = QuickSlotWidgetClassFinder.Class;
	}

	// 대화 위젯 클래스
	static ConstructorHelpers::FClassFinder<UIVDialogueWidget> DialogueWidgetClassFinder
	(TEXT("/Game/Widget/WBP_Dialogue.WBP_Dialogue_C"));
	if (DialogueWidgetClassFinder.Class)
	{
		DialogueWidgetClass = DialogueWidgetClassFinder.Class;
	}

	bOnDialogueMode = false;
}

void AIVSimpleStatHUD::BeginPlay()
{
	Super::BeginPlay();

	// 0번 : 타겟팅 마커 위젯 생성 - 가장 아래
	if (TargetMarkerWidgetClass)
	{
		TargetMarkerWidget = CreateWidget<UUserWidget>(GetWorld(), TargetMarkerWidgetClass);
		TargetMarkerWidget->AddToViewport(0);
		TargetMarkerWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 1번 : 플레이어 스텟 표시용 위젯 생성 - HUD 상시 표기
	if (PlayerStatWidgetClass)
	{
		PlayerStatWidget = CreateWidget<UIVSimpleStatWidget>(GetWorld(), PlayerStatWidgetClass);
		PlayerStatWidget->AddToViewport(1);
		PlayerStatWidget->SetVisibility(ESlateVisibility::Visible);
	}

	// 2번 : 퀵 슬롯 위젯 생성 - HUD 상시 표기
	if (QuickSlotWidgetClass)
	{
		QuickSlotWidget = CreateWidget<UIVQuickSlotWidget>(GetWorld(), QuickSlotWidgetClass);
		QuickSlotWidget->AddToViewport(2);
		QuickSlotWidget->SetVisibility(ESlateVisibility::Visible);
	}

	// 3번 : 보스 스텟 표시용 위젯 생성 - 활성화 시 표기
	if (BossStatWidgetClass)
	{
		BossStatWidget = CreateWidget<UIVSimpleBossStatWidget>(GetWorld(), BossStatWidgetClass);
		BossStatWidget->AddToViewport(3);
		BossStatWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 4번 : 사망 시 표시할 위젯 생성 - 활성화 시 표기
	if (DeathWidgetClass)
	{
		DeathWidget = CreateWidget<UUserWidget>(GetWorld(), DeathWidgetClass);
		DeathWidget->AddToViewport(4); 
		DeathWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 5번 : 보스 클리어 위젯 생성 - 활성화 시 표기
	if (BossClearWidgetClass)
	{
		BossClearWidget = CreateWidget<UUserWidget>(GetWorld(), BossClearWidgetClass);
		BossClearWidget->AddToViewport(5);
		BossClearWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 6번 : 인벤토리 위젯 생성 - 활성화 시 표기
	if (InventoryBaseWidgetClass)
	{
		InventoryBaseWidget = CreateWidget<UIVInventoryBaseWidget>(GetWorld(), InventoryBaseWidgetClass);
		InventoryBaseWidget->AddToViewport(6); 
		InventoryBaseWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 7번 : 메뉴 위젯 생성 - 활성화 시 표기, 현재 사용하지 않음
	if (MenuWidgetClass)
	{
		MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
		MenuWidget->AddToViewport(7); 
		MenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 8번 : 대화 위젯 생성 - 활성화 시 표기
	if (DialogueWidgetClass)
	{
		DialogueWidget = CreateWidget<UIVDialogueWidget>(GetWorld(), DialogueWidgetClass);
		DialogueWidget->AddToViewport(8);
		DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
		DialogueWidget->OnButtonChoicedEvent.BindDynamic(this, &AIVSimpleStatHUD::OnChiceNextDialogue);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Dialogue Widget Class Not Found"));
	}


	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
	{
		// 사망 및 부활 이벤트 서브시스템 등록
		UIVDeathEventSubsystem* LifeEventSubsystem = GameInstance->GetSubsystem<UIVDeathEventSubsystem>();
		if (LifeEventSubsystem)
		{
			LifeEventSubsystem->PlayerDeathEventDelegate.AddUObject(this, &AIVSimpleStatHUD::OnPlayerDeath);
			LifeEventSubsystem->PlayerRespawnEventDelegate.AddUObject(this, &AIVSimpleStatHUD::OnPlayerAlive);
			LifeEventSubsystem->PlayerRespawnCompleteDelegate.AddUObject(this, &AIVSimpleStatHUD::BindPlayerStatWidget);
			LifeEventSubsystem->MonsterDeathEventDelegate.AddUObject(this, &AIVSimpleStatHUD::OnBossDeath);
			LifeEventSubsystem->MonsterDeathEventDelegate.AddUObject(this, &AIVSimpleStatHUD::OnTargetDeath);
		}

		// 대화 이벤트 서브시스템 등록
		UIVDialogueManagerSubsystem* DialogueManagerSubsystem = GameInstance->GetSubsystem<UIVDialogueManagerSubsystem>();
		if (DialogueManagerSubsystem)
		{
			DialogueManagerSubsystem->OnDialogueModeSet.AddUObject(this, &AIVSimpleStatHUD::SetDialogueMode);
			DialogueManagerSubsystem->OnDialogueReady.BindUObject(this, &AIVSimpleStatHUD::UpdateDialogue);
			ContinueDialogueDelegate.BindUObject(DialogueManagerSubsystem, &UIVDialogueManagerSubsystem::ContinueDialogue);
		}
	}

	// 인벤토리 바인딩
	APawn* PlayerPawn = GetOwningPawn();
	if (PlayerPawn && PlayerPawn->Implements<UIIVInventoryComponentProvider>())
	{
		if (IIIVInventoryComponentProvider* Provider = Cast<IIIVInventoryComponentProvider>(PlayerPawn))
		{
			if (UIVInventoryComponent* InventoryComponent = Provider->GetInventoryComponent())
			{
				// 인벤토리 Base 위젯에 해당 인벤토리 컴포넌트 전달 및 바인딩
				if (InventoryBaseWidget)
				{
					InventoryBaseWidget->InventoryComponent = InventoryComponent;
					InventoryBaseWidget->InventoryWidget->InventoryComponent = InventoryComponent;
					InventoryBaseWidget->InventoryWidget->InitializeInventorySlots();

					InventoryComponent->OnInventorySlotUpdated.AddDynamic(InventoryBaseWidget->InventoryWidget, &UIVInventoryWidget::UpdateInventorySlots);
					InventoryComponent->OnQuickSlotUpdated.AddDynamic(InventoryBaseWidget->InventoryWidget, &UIVInventoryWidget::UpdateQuickSlots);
					InventoryComponent->OnEquipSlotUpdated.AddDynamic(InventoryBaseWidget->InventoryWidget, &UIVInventoryWidget::UpdateEquipSlots);
					InventoryComponent->OnWeaponSlotUpdated.AddDynamic(InventoryBaseWidget->InventoryWidget, &UIVInventoryWidget::UpdateWeaponSlot);
				}

				// 퀵 슬롯 위젯에 해당 인벤토리 컴포넌트 전달 및 바인딩
				if (QuickSlotWidget)
				{
					QuickSlotWidget->SetInventoryComponent(InventoryComponent);
					InventoryComponent->OnQuickSlotUpdated.AddDynamic(QuickSlotWidget, &UIVQuickSlotWidget::UpdateQuickSlots);
				}
			}
		}
	}
}

void AIVSimpleStatHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bShowTargetMarker && TargetActor && TargetMarkerWidget)
	{
		FVector2D ScreenLocation;
		if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayerController(), TargetActor->GetActorLocation(), ScreenLocation))
		{
			TargetMarkerWidget->SetPositionInViewport(ScreenLocation);
		}
	}
}

void AIVSimpleStatHUD::OnPlayerDeath()
{
	// 타겟 마커 숨기고
	HideTargetMarker();

	// 플레이어 스탯 위젯 숨기고
	if(PlayerStatWidget)
	{
		PlayerStatWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 사망 위젯 만들어서 띄우기
	if (DeathWidget)
	{
		DeathWidget->SetVisibility(ESlateVisibility::Visible);
	}

	// 사망 사운드 재생 - 부활 시 종료를 위한 SpawnSound2D 및 컴포넌트 저장
	if (PlayerDeathWidgetSound)
	{
		PlayerDeathAudioComponent = UGameplayStatics::SpawnSound2D(this, PlayerDeathWidgetSound);
	}
}

void AIVSimpleStatHUD::OnPlayerAlive()
{
	// 플레이어 스탯 위젯 띄우고
	if (PlayerStatWidget)
	{
		PlayerStatWidget->SetVisibility(ESlateVisibility::Visible);
	}

	// 사망 위젯 제거하기
	if (DeathWidget)
	{
		DeathWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 사망 사운드 중지
	if (PlayerDeathWidgetSound)
	{
		PlayerDeathAudioComponent->Stop();
	}
}

void AIVSimpleStatHUD::BindPlayerStatWidget()
{
	// 스탯 정보 갱신용 플레이어의 스탯 컴포넌트 가져오기
	APawn* PlayerPawn = GetOwningPawn();
	if (PlayerPawn && Cast<IIIVCharacterComponentProvider>(PlayerPawn))
	{
		IIIVCharacterComponentProvider* Provider = Cast<IIIVCharacterComponentProvider>(PlayerPawn);
		CharacterStatComponent = Provider->GetCharacterStatComponent();
	}

	// 위젯 갱신 함수와 스탯 컴포넌트 바인딩
	if (CharacterStatComponent && PlayerStatWidget)
	{
		CharacterStatComponent->OnBaseStatChanged.BindUObject(PlayerStatWidget, &UIVSimpleStatWidget::UpdatePlayerStatBars);
	
		// 현재 스탯 가져와서 위젯에 적용
		PlayerStatWidget->UpdatePlayerStatBars(CharacterStatComponent->GetBaseStat());
	}
}

void AIVSimpleStatHUD::OnBossDeath(AActor* DeadMonster)
{
	if (BossEnemy && BossEnemy == DeadMonster)
	{
		HideBossStatWidget();

		// 보스 클리어 위젯 띄우기
		if (BossClearWidget)
		{
			BossClearWidget->SetVisibility(ESlateVisibility::Visible);

			// 5초 후에 숨기기
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
				{
					if (BossClearWidget)
					{
						BossClearWidget->SetVisibility(ESlateVisibility::Collapsed);
					}
				}), 5.0f, false);
		}

		// 보스 클리어 사운드 재생 - 중간 종료 없으니 PlaySound2D 사용
		if (BossClearWidgetSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), BossClearWidgetSound);
		}
	}
}

void AIVSimpleStatHUD::ShowBossStatWidget(AIVBossEnemy* Boss)
{
	BossEnemy = Boss;
	if (BossStatWidget && Boss)
	{
		// 이름 업데이트
		BossStatWidget->UpdateBossName(Boss->GetMonsterName());

		// 체력 바인딩 및 업데이트 
		IIIVMonsterComponentProvider* Provider = Cast<IIIVMonsterComponentProvider>(Boss);
		BossStatComponent = Provider->GetMonsterStatComponent();

		if (BossStatComponent)
		{
			BossStatComponent->OnHpChanged.BindUObject(BossStatWidget, &UIVSimpleBossStatWidget::UpdateBossHpBar);
			BossStatWidget->UpdateBossHpBar(BossStatComponent->GetMonsterMaxHp(), BossStatComponent->GetMonsterCurrentHp());
		}

		// 위젯 보이기
		BossStatWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AIVSimpleStatHUD::HideBossStatWidget()
{
	// 체력 바인딩 해제
	if (BossStatComponent)
	{
		BossStatComponent->OnHpChanged.Unbind();
	}
	BossStatComponent = nullptr;
	BossEnemy = nullptr;

	// 위젯 숨기기
	if (BossStatWidget)
	{
		BossStatWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AIVSimpleStatHUD::OnTargetDeath(AActor* DeadTarget)
{
	if (TargetActor && TargetActor == DeadTarget)
	{
		HideTargetMarker();
	}
}

void AIVSimpleStatHUD::ShowTargetMarker(AActor* Target)
{
	TargetActor = Target;
	bShowTargetMarker = true;
	
	if (TargetMarkerWidget)
	{
		TargetMarkerWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AIVSimpleStatHUD::HideTargetMarker()
{
	TargetActor = nullptr;
	bShowTargetMarker = false;

	if (TargetMarkerWidget)
	{
		TargetMarkerWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AIVSimpleStatHUD::ShowInventory()
{
	if (InventoryBaseWidget)
	{
		InventoryBaseWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AIVSimpleStatHUD::HideInventory()
{
	if (InventoryBaseWidget)
	{
		InventoryBaseWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AIVSimpleStatHUD::ShowMenu()
{
}

void AIVSimpleStatHUD::HideMenu()
{
}

void AIVSimpleStatHUD::SetDialogueMode(bool bDialogueMode)
{
	if (bDialogueMode != bOnDialogueMode)
	{
		if (bDialogueMode) // 대화모드로 전환
		{
			if (PlayerStatWidget)
			{
				PlayerStatWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			if (QuickSlotWidget)
			{
				QuickSlotWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			if (DialogueWidget)
			{
				DialogueWidget->SetVisibility(ESlateVisibility::Visible);
			}
			bOnDialogueMode = true;
		}
		else // 대화모드 해제
		{
			if(PlayerStatWidget)
			{
				PlayerStatWidget->SetVisibility(ESlateVisibility::Visible);
			}
			if (QuickSlotWidget)
			{
				QuickSlotWidget->SetVisibility(ESlateVisibility::Visible);
			}
			if (DialogueWidget)
			{
				DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			bOnDialogueMode = false;
		}
	}
}

void AIVSimpleStatHUD::UpdateDialogue(const FDialogueInfo& CurrentDialogue)
{
	if (DialogueWidget)
	{
		DialogueWidget->UpdateDialogue(CurrentDialogue);
	}
}

void AIVSimpleStatHUD::OnChiceNextDialogue(const FName& NextDialogueID)
{
	ContinueDialogueDelegate.ExecuteIfBound(NextDialogueID);
}
