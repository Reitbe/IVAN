// Fill out your copyright notice in the Description page of Project Settings.


#include "IVPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "IVAN/UI/IVSimpleStatHUD.h"
#include "IVAN/GameSystem/IVDeathEventSubsystem.h"
#include "IVAN/GameSystem/IVDialogueManagerSubsystem.h"

AIVPlayerController::AIVPlayerController()
{
	// 입력 관련 에셋들 초기화
	InputConstructHelper();

	bShowMenu = false;
	bShowInventory = false;
	bOnDialogueMode = false;
}

// 입력 관련 에셋들은 생성자에서 초기화 및 로드
void AIVPlayerController::InputConstructHelper()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextFinder
	(TEXT("/Game/Input/IMC_PlayerController.IMC_PlayerController"));
	if (InputMappingContextFinder.Succeeded())
	{
		InputMappingContext = InputMappingContextFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShowMenuActionFinder
	(TEXT("/Game/Input/Actions/IA_Menu.IA_Menu"));
	if (ShowMenuActionFinder.Succeeded())
	{
		ShowMenuAction = ShowMenuActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShowInventoryActionFinder
	(TEXT("/Game/Input/Actions/IA_Inventory.IA_Inventory"));
	if (ShowInventoryActionFinder.Succeeded())
	{
		ShowInventoryAction = ShowInventoryActionFinder.Object;
	}
}

void AIVPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 입력 컨텍스트 등록
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (InputSubsystem != nullptr) {
		InputSubsystem->AddMappingContext(InputMappingContext, 0); // 캐릭터는 Priority가 1이다
	}

	
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
	{
		// 사망 및 부활 이벤트를 서브시스템에 바인딩
		UIVDeathEventSubsystem* DeathEventSubsystem = GameInstance->GetSubsystem<UIVDeathEventSubsystem>();
		if (DeathEventSubsystem)
		{
			DeathEventSubsystem->PlayerDeathEventDelegate.AddUObject(this, &AIVPlayerController::SetDead);
			DeathEventSubsystem->PlayerRespawnEventDelegate.AddUObject(this, &AIVPlayerController::SetAlive);
		}

		// 대화 모드로의 전환을 위한 대화 서브시스템 바인딩
		UIVDialogueManagerSubsystem* DialogueManagerSubsystem = GameInstance->GetSubsystem<UIVDialogueManagerSubsystem>();
		if (DialogueManagerSubsystem)
		{
			DialogueManagerSubsystem->OnDialogueModeSet.AddUObject(this, &AIVPlayerController::SetDialogueMode);
		}
	}

	// HUD 획득
	SimpleStatHUD = Cast<AIVSimpleStatHUD>(GetHUD());
}

void AIVPlayerController::SetDead()
{
	// 입력 관련 비활성화
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
	SetIgnoreMoveInput(true);
	GetPawn()->DisableInput(this);
}

void AIVPlayerController::SetAlive()
{
	// 입력 관련 활성화
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
	SetIgnoreMoveInput(false);
	GetPawn()->EnableInput(this);
}

void AIVPlayerController::ShowBossStatWidget(AIVBossEnemy* Boss)
{
	if (SimpleStatHUD)
	{
		SimpleStatHUD->ShowBossStatWidget(Boss);
	}
}

void AIVPlayerController::HideBossStatWidget()
{
	if (SimpleStatHUD)
	{
		SimpleStatHUD->HideBossStatWidget();
	}
}

void AIVPlayerController::ShowTargetMarker(AActor* Target)
{
	if (SimpleStatHUD)
	{
		SimpleStatHUD->ShowTargetMarker(Target);
	}
}

void AIVPlayerController::HideTargetMarker()
{
	if (SimpleStatHUD)
	{
		SimpleStatHUD->HideTargetMarker();
	}
}

void AIVPlayerController::SetDialogueMode(bool bDialogueMode)
{
	if (bDialogueMode != bOnDialogueMode)
	{
		if (bDialogueMode) // 대화모드로 전환
		{
			SetShowMouseCursor(true);
			GetPawn()->DisableInput(this);
			bOnDialogueMode = true;
		
		}
		else // 대화모드 해제
		{
			SetShowMouseCursor(false);
			GetPawn()->EnableInput(this);
			bOnDialogueMode = false;
		}
	}
}

void AIVPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindAction(ShowMenuAction, ETriggerEvent::Started, this, &AIVPlayerController::ShowMenu);
		Input->BindAction(ShowInventoryAction, ETriggerEvent::Started, this, &AIVPlayerController::ShowOrHideInventory);
	}
}

void AIVPlayerController::ShowMenu()
{
	if (bShowMenu)
	{
		bShowMenu = false;
	}
	else
	{
		bShowMenu = true;
	}
}

void AIVPlayerController::ShowOrHideInventory()
{
	if (SimpleStatHUD)
	{
		if (bShowInventory) // 인벤토리 닫기
		{
			SimpleStatHUD->HideInventory();
			SetShowMouseCursor(false);
			GetPawn()->EnableInput(this);
			bShowInventory = false;
		}
		else // 인벤토리 열기
		{
			SimpleStatHUD->ShowInventory();
			SetShowMouseCursor(true);
			GetPawn()->DisableInput(this);
			bShowInventory = true;
		}
	}
}

