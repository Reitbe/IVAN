// Fill out your copyright notice in the Description page of Project Settings.


#include "IVPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterTrajectoryComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "IVAN/Stat/IVCharacterStatComponent.h"

AIVPlayerCharacter::AIVPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 입력 관련 에셋들 초기화
	InputConstructHelper();

	// 몽타주 관련 에셋들 초기화
	MontageConstructHelper();

	// 모션 매칭용 추적 컴포넌트
	TrajectoryComponent = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("CharacterTrajectoryComponent"));

	// 캐릭터 스탯 상호작용을 위한 컴포넌트
	CharacterStatComponent = CreateDefaultSubobject<UIVCharacterStatComponent>(TEXT("CharacterStatComponent"));

	// 카메라 관련 컴포넌트 설정
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
}

// 입력 관련 에셋들은 생성자에서 초기화 및 로드
void AIVPlayerCharacter::InputConstructHelper()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextFinder
	(TEXT("/Game/Input/IMC_Player.IMC_Player"));
	if (InputMappingContextFinder.Succeeded())
	{
		InputMappingContext = InputMappingContextFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> BasicAttackActionFinder
	(TEXT("/Game/Input/Actions/IA_BasicAttack.IA_BasicAttack"));
	if (BasicAttackActionFinder.Succeeded())
	{
		BasicAttackAction = BasicAttackActionFinder.Object;
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

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionFinder 
	(TEXT("/Game/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpActionFinder.Succeeded())
	{
		JumpAction = JumpActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RunWalkSwitchActionFinder
	(TEXT("/Game/Input/Actions/IA_RunWalkSwitch.IA_RunWalkSwitch"));
	if (RunWalkSwitchActionFinder.Succeeded())
	{
		RunWalkSwitchAction = RunWalkSwitchActionFinder.Object;
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

void AIVPlayerCharacter::MontageConstructHelper()
{
	/*
	* 애님 인스턴스는 Blueprint에서 연결한다. 관련된 에셋들이 많기 때문이다.
	* 몽타주는 별도로 BeginPlay에서 로드한다.
	*/

	RollMontage = TSoftObjectPtr<UAnimMontage>(FSoftObjectPath(
		TEXT("/Game/Animation/Retarget/Roll/Roll_Edit_Montage.Roll_Edit_Montage"))
	);

	DodgeRightMontage = TSoftObjectPtr<UAnimMontage>(FSoftObjectPath(
		TEXT(""))
	);

	DodgeLeftMontage = TSoftObjectPtr<UAnimMontage>(FSoftObjectPath(
		TEXT(""))
	);

	DodgeBackMontage = TSoftObjectPtr<UAnimMontage>(FSoftObjectPath(
		TEXT(""))
	);
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

	
	AnimInstance = GetMesh()->GetAnimInstance();

	RollMontage.LoadSynchronous();		// 몽타주 로드
	DodgeRightMontage.LoadSynchronous();// 미구현
	DodgeLeftMontage.LoadSynchronous(); // 미구현
	DodgeBackMontage.LoadSynchronous(); // 미구현

	// 캐릭터 속도 초기화
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
}

void AIVPlayerCharacter::Tick(float DeltaTime)
{
}

void AIVPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 이동 및 시야
		Input->BindAction(BasicMovement, ETriggerEvent::Triggered, this, &AIVPlayerCharacter::BasicMove);
		Input->BindAction(SpecialMovement, ETriggerEvent::Triggered, this, &AIVPlayerCharacter::SpecialMove);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump); // ACharacter의 Jump 함수 사용
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AIVPlayerCharacter::Look);

		// 전환
		Input->BindAction(RunWalkSwitchAction, ETriggerEvent::Triggered, this, &AIVPlayerCharacter::RunWalkSwitch);

		// 공격
		Input->BindAction(BasicAttackAction, ETriggerEvent::Triggered, this, &AIVPlayerCharacter::BasicAttack);
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
	// 이미 특수 동작 중이거나 공중에 떠 있을 때는 실행하지 않는다.
	bool bInAir = GetCharacterMovement()->IsFalling();
	bool bIsRolling = CharacterStatComponent->GetCharacterSpecialMovementState() == ESpecialMovementState::Rolling;

	if (!bInAir && !bIsRolling) 
	{
		if (AnimInstance && CharacterStatComponent)
		{
			AnimInstance->Montage_Play(RollMontage.Get());
			CharacterStatComponent->SetCharacterSpecialMoveState(ESpecialMovementState::Rolling); // 구르기 시작할 때 상태 변경

			FOnMontageBlendingOutStarted BlendingOutDelegate;
			BlendingOutDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
				{
					CharacterStatComponent->SetCharacterSpecialMoveState(ESpecialMovementState::None); // 구르기 끝날 때 상태 변경
				});
			AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, RollMontage.Get());
		}
	}
}

void AIVPlayerCharacter::RunWalkSwitch(const FInputActionValue& Value)
{
	if (CharacterStatComponent)
	{
		if (CharacterStatComponent->GetCharacterGaitState() == EGaitState::Run) // 달리기->걷기
		{
			CharacterStatComponent->SetCharacterGaitState(EGaitState::Walk);
			GetCharacterMovement()->MaxWalkSpeed /= 2;
		}
		else // 걷기 -> 달리기
		{
			CharacterStatComponent->SetCharacterGaitState(EGaitState::Run);
			GetCharacterMovement()->MaxWalkSpeed *= 2;
		}
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

void AIVPlayerCharacter::BasicAttack(const FInputActionValue& Value)
{
	// [임시구현] 캐릭터를 중심으로 구 범위에 있는 적에게 데미지를 준다. 그리고 그 범위를 표시한다.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Basic Attack"));
	UGameplayStatics::ApplyRadialDamage(this, 50.0f, GetActorLocation(), 300.0f, nullptr, TArray<AActor*>(), this, nullptr, true, ECC_Visibility);
	DrawDebugSphere( // 데미지 범위 표시
		GetWorld(),              // 월드 컨텍스트
		GetActorLocation(),      // 중심 위치
		300.0f,                  // 반경
		12,                      // 세그먼트 수
		FColor::Red,             // 색상
		false,                   // 영구적으로 그릴지 여부
		3.0f                     // 지속 시간 (초)
	);
}

float AIVPlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 캐릭터 스탯 컴포넌트에 데미지 전달
	CharacterStatComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return 0.0f;
}


