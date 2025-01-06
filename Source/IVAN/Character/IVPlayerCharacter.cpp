// Fill out your copyright notice in the Description page of Project Settings.


#include "IVPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

AIVPlayerCharacter::AIVPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 입력 관련 에셋들 초기화
	InputConstructHelper();

	// 카메라 관련 컴포넌트 설정
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
}

void AIVPlayerCharacter::InputConstructHelper()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextFinder
	(TEXT("/Game/Input/IMC_Player.IMC_Player"));
	if (InputMappingContextFinder.Succeeded())
	{
		InputMappingContext = InputMappingContextFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> BasicAttackFinder
	(TEXT(""));
	if (BasicAttackFinder.Succeeded())
	{
		BasicAttack = BasicAttackFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SpecialAttackFinder
	(TEXT(""));
	if (SpecialAttackFinder.Succeeded())
	{
		SpecialAttack = SpecialAttackFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> BasicMovementFinder
	(TEXT("/Game/Input/Actions/IA_Move.IA_Move"));
	if (BasicMovementFinder.Succeeded())
	{
		BasicMovement = BasicMovementFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SpecialMovementFinder
	(TEXT("/Game/Input/Actions/IA_SpecialMove.IA_SpecialMove"));
	if (SpecialMovementFinder.Succeeded())
	{
		SpecialMovement = SpecialMovementFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionFinder
	(TEXT("/Game/Input/Actions/IA_Look.IA_Look"));
	if (LookActionFinder.Succeeded())
	{
		LookAction = LookActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TargetActionFinder
	(TEXT(""));
	if (TargetActionFinder.Succeeded())
	{
		TargetAction = TargetActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InteractActionFinder
	(TEXT(""));
	if (InteractActionFinder.Succeeded())
	{
		InteractAction = InteractActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> DefendActionFinder
	(TEXT(""));
	if (DefendActionFinder.Succeeded())
	{
		DefendAction = DefendActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> UseFirstItemSlotFinder
	(TEXT(""));
	if (UseFirstItemSlotFinder.Succeeded())
	{
		UseFirstItemSlot = UseFirstItemSlotFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> UseSecondItemSlotFinder
	(TEXT(""));
	if (UseSecondItemSlotFinder.Succeeded())
	{
		UseSecondItemSlot = UseSecondItemSlotFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> UseThirdItemSlotFinder
	(TEXT(""));
	if (UseThirdItemSlotFinder.Succeeded())
	{
		UseThirdItemSlot = UseThirdItemSlotFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> UseFourthItemSlotFinder
	(TEXT(""));
	if (UseFourthItemSlotFinder.Succeeded())
	{
		UseFourthItemSlot = UseFourthItemSlotFinder.Object;
	}
}

void AIVPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 입력 컨텍스트 등록
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(InputMappingContext, 1); // PlayerController의 우선순위는 0이다.
		}
	}
}

void AIVPlayerCharacter::Tick(float DeltaTime)
{
}

void AIVPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Input->BindAction(BasicAttack, ETriggerEvent::Started, this, &AIVPlayerCharacter::BasicMove);
		//Input->BindAction(SpecialAttack, ETriggerEvent::Started, this, &AIVPlayerCharacter::SpecialMove);
		Input->BindAction(BasicMovement, ETriggerEvent::Triggered, this, &AIVPlayerCharacter::BasicMove);
		Input->BindAction(SpecialMovement, ETriggerEvent::Triggered, this, &AIVPlayerCharacter::SpecialMove);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AIVPlayerCharacter::Look);
	}
}

void AIVPlayerCharacter::BasicMove(const FInputActionValue& Value)
{
	if (PlayerController)
	{
		// 크기
		float FowardValue = Value.Get<FVector2D>().Y;
		float SideValue = Value.Get<FVector2D>().X;

		// 방향
		const FRotator Rotation = PlayerController->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 축
		FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector SideDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(FowardDirection, FowardValue);
		AddMovementInput(SideDirection, SideValue);
	}
}

void AIVPlayerCharacter::SpecialMove(const FInputActionValue& Value)
{
	// 스페이스바를 한 번 누르면 구르기, 스페이스바를 길게 누르면 전력질주, 스페이스바를 길게 누른 상태에서 한 번 더 누르면 점프
	if (PlayerController)
	{
		Jump();
		//float PressedTime = Value.Get<float>();

		//if (PressedTime < 0.2f) // 구르기
		//{
		//	//Roll();
		//}
		//else if (PressedTime >= 0.2f) // 전력질주
		//{
		//	//Sprint();
		//}
		//else if (PressedTime >= 1.0f)
		//{
		//	// 점프(Character 클래스)
		//	
		//}
	}
}

void AIVPlayerCharacter::Look(const FInputActionValue& Value)
{
	if (PlayerController)
	{
		AddControllerYawInput(Value.Get<FVector2D>().X);
		AddControllerPitchInput(Value.Get<FVector2D>().Y);
	}
}


