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
#include "IVAN/Stat/IVEquipComponent.h"
#include "IVAN/Attack/IVHitReactionComponent.h"
#include "IVAN/Attack/IVAttackComponent.h"
#include "IVAN/GameSystem/IVDeathEventSubsystem.h"
#include "IVAN/Item/IVWeapon.h"

AIVPlayerCharacter::AIVPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 입력 관련 에셋들 로드 및 초기화
	InputConstructHelper();

	// 몽타주 관련 에셋들 로드 및 초기화
	MontageConstructHelper();

	// 장비 관리용 컴포넌트
	EquipComponent = CreateDefaultSubobject<UIVEquipComponent>(TEXT("EquipComponent"));

	// 모션 매칭용 추적 컴포넌트
	TrajectoryComponent = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("CharacterTrajectoryComponent"));

	// 스탯 컴포넌트 추가
	CharacterStatComponent = CreateDefaultSubobject<UIVCharacterStatComponent>(TEXT("CharacterStatComponent"));

	// 공격 및 피격 관련 컴포넌트
	AttackComponent = CreateDefaultSubobject<UIVAttackComponent>(TEXT("AttackComponent"));
	HitReactionComponent = CreateDefaultSubobject<UIVHitReactionComponent>(TEXT("HitReactionComponent"));

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

void AIVPlayerCharacter::EquipByClass(TSubclassOf<AIVItemBase> Item) const
{
}

void AIVPlayerCharacter::EquipByInstance(TObjectPtr<AIVItemBase> Item) const
{
	// 전달받은 아이템을 지정 위치에 부착한다
	if (Item)
	{
		/* 
		* 아이템은 본인이 장착될 위치를 가지고 있다. 
		* 현재는 임시로 플레이어 캐릭터의 특정 소켓을 사용한다.
		*/
		FName SocketName(TEXT("hand_rSocket"));
		if (!GetMesh()->DoesSocketExist(SocketName))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("캐릭터에 해당 소켓이 없습니다."));
			return;
		}
		Item->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	}

	// 아이템이 무기라면 공격 컴포넌트에 무기를 설정한다
	if (Item->GetItemType() == EItemType::Weapon && AttackComponent)
	{
		AttackComponent->SetWeapon(Cast<AIVWeapon>(Item));
	}
}

void AIVPlayerCharacter::StartHitReaction()
{
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	if (CharacterStatComponent)
	{
		// 어떠한 상태더라도 피격 상태로 변경
		CharacterStatComponent->SetCharacterSpecialMoveState(ESpecialMovementState::HitStunned);
	}
}

void AIVPlayerCharacter::EndHitReaction()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	if (CharacterStatComponent)
	{
		CharacterStatComponent->SetCharacterSpecialMoveState(ESpecialMovementState::None);
	}
}

void AIVPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<APlayerController>(GetController());

	// 입력 컨텍스트 등록
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(InputMappingContext, 1); // PlayerController의 우선순위는 0이다.
		}
	}

	// 사망 및 부활 이벤트를 서브시스템에 바인딩
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
	{
		UIVDeathEventSubsystem* DeathEventSubsystem = GameInstance->GetSubsystem<UIVDeathEventSubsystem>();
		if (DeathEventSubsystem)
		{
			DeathEventSubsystem->PlayerDeathEventDelegate.AddUObject(this, &AIVPlayerCharacter::SetDead);
			DeathEventSubsystem->PlayerRespawnEventDelegate.AddUObject(this, &AIVPlayerCharacter::SetAlive);
		}
	}

	// 애님 인스턴스 설정
	AnimInstance = GetMesh()->GetAnimInstance();
	RollMontage.LoadSynchronous();

	// 공격 및 피격 시스템에 애님 인스턴스 설정
	if (AnimInstance)
	{
		HitReactionComponent->SetAnimInstance(AnimInstance);
		AttackComponent->SetAnimInstance(AnimInstance);
	}

	// 캐릭터 이동 속도 초기화
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
}

TObjectPtr<AIVWeapon> AIVPlayerCharacter::GetWeapon() const
{
	// 장비 컴포넌트로부터 무기를 받아 전달한다
	if (EquipComponent)
	{
		return EquipComponent->GetWeapon();
	}
	return nullptr;
}

void AIVPlayerCharacter::SetWeaponOnWeaponComponent(TObjectPtr<AIVWeapon> Weapon)
{
	// 장비 컴포넌트로부터 무기를 받아 공격 컴포넌트에 전달한다. 
	if (AttackComponent)
	{
		AttackComponent->SetWeapon(EquipComponent->GetWeapon());
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
	// 이미 특수 동작 중이거나 공중에 떠 있을 때는 새로운 동작을 실행하지 않는다.
	bool bInAir = GetCharacterMovement()->IsFalling();
	bool bInSpecialMove = CharacterStatComponent->GetCharacterSpecialMovementState() != ESpecialMovementState::None;

	if (!bInAir && !bInSpecialMove)
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
		if (CharacterStatComponent->GetCharacterGaitState() == EGaitState::Run) // 달리기 -> 걷기
		{
			CharacterStatComponent->SetCharacterGaitState(EGaitState::Walk);
			GetCharacterMovement()->MaxWalkSpeed /= 2;
		}
		else																	// 걷기 -> 달리기
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
	// 특수 동작 중이거나 공중에 떠 있을 때는 공격을 실행하지 않는다
	bool bInAir = GetCharacterMovement()->IsFalling();
	bool bInSpecialMove = CharacterStatComponent->GetCharacterSpecialMovementState() != ESpecialMovementState::None;
	if (bInAir || bInSpecialMove) return; 
	
	// 스탯 컴포넌트로부터 캐릭터의 데미지 정보를 가져온다
	FBaseDamageStat BaseDamageStat;
	if (CharacterStatComponent)
	{
		CharacterStatComponent->SetCharacterSpecialMoveState(ESpecialMovementState::Attacking); 
		BaseDamageStat = CharacterStatComponent->GetBaseDamageStat();
	}

	// 가져온 캐릭터의 데미지 정보를 공격 컴포넌트에 전달한다
	if(AttackComponent)
	{
		AttackComponent->Attack(BaseDamageStat);
	}
}

void AIVPlayerCharacter::AttackEnd(bool bIsFirstCheck)
{
	if (CharacterStatComponent)
	{
		// 정상적으로 몽타주가 종료되어 종료 노티파이가 호출된 경우 -> 2회차 검사 생략
		if (bIsFirstCheck)
		{
			CharacterStatComponent->SetCharacterSpecialMoveState(ESpecialMovementState::None);
			bAttackEndChecked = true;
		}
		// 2회차 검사는 1회차 검사 여부와 관계 없이 항상 진행
		else
		{
			// 1회차를 패스하고 2회차 검사를 진행하는 경우
			if (!bAttackEndChecked)
			{
				CharacterStatComponent->SetCharacterSpecialMoveState(ESpecialMovementState::None);
			}
			// 1회차 검사를 통과한 경우 -> 패스
			bAttackEndChecked = false; // 2회차 검사가 끝나면 항상 공격 종료 체크를 초기화해야한다.  
		}
	}	

	if (AttackComponent)
	{
		AttackComponent->AttackEnd();
	}
}

void AIVPlayerCharacter::SetDead()
{
	Super::SetDead();

	if (PlayerController)
	{
		DisableInput(PlayerController);
	}
}

void AIVPlayerCharacter::SetAlive()
{
	Super::SetAlive();

	if (PlayerController)
	{
		EnableInput(PlayerController);
	}
}

float AIVPlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// 데미지 처리 및 피격 리액션 진행
	CharacterStatComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	HitReactionComponent->ComputeHitAngle(Damage, DamageEvent, EventInstigator, DamageCauser);
	StartHitReaction();
	return 0.0f;
}


