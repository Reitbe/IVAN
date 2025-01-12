// Fill out your copyright notice in the Description page of Project Settings.


#include "IVPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

AIVPlayerController::AIVPlayerController()
{
	// 입력 관련 에셋들 초기화
	InputConstructHelper();
}

// 입력 관련 에셋들은 생성자에서 초기화 및 로드
void AIVPlayerController::InputConstructHelper()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextFinder
	(TEXT(""));
	if (InputMappingContextFinder.Succeeded())
	{
		InputMappingContext = InputMappingContextFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShowMenuActionFinder
	(TEXT(""));
	if (ShowMenuActionFinder.Succeeded())
	{
		ShowMenuAction = ShowMenuActionFinder.Object;
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
}

void AIVPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindAction(ShowMenuAction, ETriggerEvent::Started, this, &AIVPlayerController::ShowMenu);
	}
}

void AIVPlayerController::ShowMenu()
{
}

