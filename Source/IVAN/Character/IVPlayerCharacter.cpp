// Fill out your copyright notice in the Description page of Project Settings.


#include "IVPlayerCharacter.h"
#include "MotionWarpingComponent.h" 
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "CharacterTrajectoryComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Components/CapsuleComponent.h"
#include "IVAN/Stat/IVCharacterStatComponent.h"
#include "IVAN/Stat/IVEquipComponent.h"
#include "IVAN/Attack/IVHitReactionComponent.h"
#include "IVAN/Attack/IVAttackComponent.h"
#include "IVAN/Attack/IVAttackRange.h"
#include "IVAN/GameSystem/IVDeathEventSubsystem.h"
#include "IVAN/GameSystem/IVConditionManagerSubsystem.h"
#include "IVAN/GameSystem/IVANGameMode.h"
#include "IVAN/Item/IVWeapon.h"
#include "IVAN/Item/IVInventoryComponent.h"
#include "IVAN/Item/IVInteractionComponent.h"
#include "IVAN/Animation/IVPlayerAnim.h"

AIVPlayerCharacter::AIVPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 입력 관련 에셋들 로드 및 초기화
	InputConstructHelper();

	// 장비 관리용 컴포넌트
	EquipComponent = CreateDefaultSubobject<UIVEquipComponent>(TEXT("EquipComponent"));

	// 인벤토리 컴포넌트
	InventoryComponent = CreateDefaultSubobject<UIVInventoryComponent>(TEXT("InventoryComponent"));

	// 상호작용 컴포넌트
	InteractionComponent = CreateDefaultSubobject<UIVInteractionComponent>(TEXT("InteractionComponent"));

	// 모션 매칭용 추적 컴포넌트
	TrajectoryComponent = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("CharacterTrajectoryComponent"));

	// 모션 워핑
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

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

	LockOnDistance = 1300.0f;
	bInComboAttack = false;
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
	(TEXT("/Game/Input/Actions/IA_LockOn.IA_LockOn"));
	if (TargetActionFinder.Succeeded())
	{
		TargetAction = TargetActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InteractActionFinder
	(TEXT("/Game/Input/Actions/IA_Interaction.IA_Interaction"));
	if (InteractActionFinder.Succeeded())
	{
		InteractAction = InteractActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> UseQuickSlot_1_Finder
	(TEXT("/Game/Input/Actions/IA_UseQuickSlot_1.IA_UseQuickSlot_1"));
	if (UseQuickSlot_1_Finder.Succeeded())
	{
		UseQuickSlot_1_Action = UseQuickSlot_1_Finder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> UseQuickSlot_2_Finder
	(TEXT("/Game/Input/Actions/IA_UseQuickSlot_2.IA_UseQuickSlot_2"));
	if (UseQuickSlot_2_Finder.Succeeded())
	{
		UseQuickSlot_2_Action = UseQuickSlot_2_Finder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> UseQuickSlot_3_Finder
	(TEXT("/Game/Input/Actions/IA_UseQuickSlot_3.IA_UseQuickSlot_3"));
	if (UseQuickSlot_3_Finder.Succeeded())
	{
		UseQuickSlot_3_Action = UseQuickSlot_3_Finder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> UseQuickSlot_4_Finder
	(TEXT("/Game/Input/Actions/IA_UseQuickSlot_4.IA_UseQuickSlot_4"));
	if (UseQuickSlot_4_Finder.Succeeded())
	{
		UseQuickSlot_4_Action = UseQuickSlot_4_Finder.Object;
	}
}

void AIVPlayerCharacter::EquipByInstance(TObjectPtr<AIVWeapon> Weapon, FName EquipSocket) const
{
	// 소켓 유무 확인 및 소켓에 무기 장착
	if (Weapon)
	{
		if (!GetMesh()->DoesSocketExist(EquipSocket)) // 보통 "hand_rSocket"에 장착한다
		{
			return;
		}
		
		if (Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, EquipSocket))
		{
			if (AttackComponent)
			{
				AttackComponent->SetWeapon(Weapon);
				AttackComponent->ProvideOwnerAttackRanges(AttackRanges); // 캐릭터가 보유한 공격 범위 제공
				Weapon->SetInteractable(false); 
			}
		}
	}
}

void AIVPlayerCharacter::UnEquipWeapon()
{
	if (AttackComponent)
	{
		AttackComponent->SetWeapon(nullptr);
	}
}

void AIVPlayerCharacter::StartHitReaction()
{
	// 어떠한 상태더라도 피격 상태로 변경
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	if (CharacterStatComponent)
	{
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

void AIVPlayerCharacter::EndDeathReaction()
{
	/*
	* 캐릭터 사망 애니메이션 재생 이후 래그돌로 전환하고자 해당 함수를 선언했다.
	* 현재는 사망 애니메이션을 재생하지 않고 오로지 래그돌만을 사용하고있다.
	*/
}

void AIVPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<AIVPlayerController>(GetController());

	// 입력 컨텍스트 등록
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(InputMappingContext, 1); // PlayerController의 우선순위는 0이다.
		}
	}

	
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
	{
		// 사망 및 부활 이벤트를 서브시스템에 바인딩
		UIVDeathEventSubsystem* DeathEventSubsystem = GameInstance->GetSubsystem<UIVDeathEventSubsystem>();
		if (DeathEventSubsystem)
		{
			DeathEventSubsystem->PlayerDeathEventDelegate.AddUObject(this, &AIVPlayerCharacter::SetDead);
			DeathEventSubsystem->PlayerRespawnEventDelegate.AddUObject(this, &AIVPlayerCharacter::SetAlive);
			DeathEventSubsystem->MonsterDeathEventDelegate.AddUObject(this, &AIVPlayerCharacter::MonsterDeath);
		}

		// 대화 및 퀘스트를 위하여 조건 확인 서브시스템에 컴포넌트 바인딩
		UIVConditionManagerSubsystem* ConditionManager = GameInstance->GetSubsystem<UIVConditionManagerSubsystem>();
		if (ConditionManager)
		{
			ConditionManager->SetInventoryComponent(InventoryComponent);
			ConditionManager->SetCharacterStatComponent(CharacterStatComponent);
		}
	}

	// 애님 인스턴스 설정
	AnimInstance = GetMesh()->GetAnimInstance();

	// 공격 및 피격 시스템에 애님 인스턴스 설정
	if (AnimInstance && HitReactionComponent && AttackComponent)
	{
		HitReactionComponent->SetAnimInstance(AnimInstance);
		AttackComponent->SetAnimInstance(AnimInstance);
	}

	// 캐릭터 이동 속도 초기화
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;

	// 캐릭터 준비 완료
	RespawnComplete();
}

void AIVPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// 보유한 공격 범위 콜라이더 찾아오기
	GetComponents<UIVAttackRange>(AttackRanges);
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
	Super::Tick(DeltaTime);

	if (CharacterStatComponent)
	{
		// 락온 중이라면 락온 대상을 따라 시점을 조정한다
		if (CharacterStatComponent->GetCharacterTargetingState() == ETargetingState::OnTargeting && LockOnActor)
		{
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockOnActor->GetActorLocation());
			FRotator NewRotation = FMath::RInterpTo(GetControlRotation(), LookAtRotation, DeltaTime, 5.0f);
			PlayerController->SetControlRotation(NewRotation);
		}
	}
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
		Input->BindAction(TargetAction, ETriggerEvent::Triggered, this, &AIVPlayerCharacter::LockOnSwitch);

		// 공격
		Input->BindAction(BasicAttackAction, ETriggerEvent::Triggered, this, &AIVPlayerCharacter::BasicAttack);

		// 상호작용
		Input->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AIVPlayerCharacter::Interact);

		// 퀵 슬롯
		Input->BindAction(UseQuickSlot_1_Action, ETriggerEvent::Triggered, this, &AIVPlayerCharacter::UseQuickSlot_1);
		Input->BindAction(UseQuickSlot_2_Action, ETriggerEvent::Triggered, this, &AIVPlayerCharacter::UseQuickSlot_2);
		Input->BindAction(UseQuickSlot_3_Action, ETriggerEvent::Triggered, this, &AIVPlayerCharacter::UseQuickSlot_3);
		Input->BindAction(UseQuickSlot_4_Action, ETriggerEvent::Triggered, this, &AIVPlayerCharacter::UseQuickSlot_4);
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

		// 이동 적용
		AddMovementInput(FowardDirection, FowardValue);
		AddMovementInput(SideDirection, SideValue);

		// 구르기용 방향 정보 설정
		MoveDirection = (FowardDirection * FowardValue) + (SideDirection * SideValue);
		MoveDirection = MoveDirection.GetSafeNormal();

		// 타겟팅 시 회전 방향 설정
		if (CharacterStatComponent && CharacterStatComponent->GetCharacterTargetingState() == ETargetingState::OnTargeting)
		{
			// 이미 컨트롤러 회전을 그대로 사용중이면 진행 X
			if (bUseControllerRotationYaw) return;

			// 캐릭터 방향을 컨트롤러 방향에 맞춰 회전
			float ControllerYaw = Rotation.Yaw;
			float CharacterYaw = GetActorRotation().Yaw;
			float InterpYaw = FMath::FInterpTo(CharacterYaw, ControllerYaw, GetWorld()->GetDeltaSeconds(), 5.0f);
			SetActorRotation(FRotator(0.0f, InterpYaw, 0.0f));

			// 회전이 어느 정도 동일해지면 컨트롤러 회전 사용 시작
			float DeltaYaw = FMath::Abs(FMath::FindDeltaAngleDegrees(CharacterYaw, ControllerYaw));
			if (DeltaYaw < 10.0f)
			{
				bUseControllerRotationYaw = true;
			}
		}
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
			// 입력 방향과 캐릭터 방향을 기준으로 어느 구르기 동작이 적합한지 판단
			FVector CharForward = GetActorForwardVector();
			FVector CharRight = GetActorRightVector();

			float FowardDot = FVector::DotProduct(CharForward, MoveDirection);
			float RightDot = FVector::DotProduct(CharRight, MoveDirection);

			float RollX = FMath::Clamp(FowardDot, -1.0f, 1.0f); // 이미 -1 ~ 1 사이 값이지만 혹시 모르니 한번 더 클램핑
			float RollY = FMath::Clamp(RightDot, -1.0f, 1.0f);

			// 방향 별 구르기 몽타주 선택
			UAnimMontage* SelectedRollMontage = nullptr;
			if (RollX >= 0.707) // cos45 = 0.707
			{
				SelectedRollMontage = RollFrontMontage;
			}
			else if (RollX <= -0.707)
			{
				SelectedRollMontage = RollBackMontage;
			}
			else if (RollY > 0.0)
			{
				SelectedRollMontage = RollRightMontage;
			}
			else if (RollY < 0.0)
			{
				SelectedRollMontage = RollLeftMontage;
			}

			// 구르기 몽타주 재생
			AnimInstance->Montage_Play(SelectedRollMontage);

			// 구르기 시작 및 종료 시 상태 변경
			CharacterStatComponent->SetCharacterSpecialMoveState(ESpecialMovementState::Rolling); // 구르기 시작할 때 상태 변경
			FOnMontageBlendingOutStarted BlendingOutDelegate;
			BlendingOutDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
				{
					CharacterStatComponent->SetCharacterSpecialMoveState(ESpecialMovementState::None); // 구르기 끝날 때 상태 변경
				});
			AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, SelectedRollMontage);
		}
	}
}

void AIVPlayerCharacter::RunWalkSwitch()
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

	bInComboAttack = true;

	// 가져온 캐릭터의 데미지 정보를 공격 컴포넌트에 전달한다
	if(AttackComponent)
	{
		AttackComponent->Attack(BaseDamageStat);
	}
}

void AIVPlayerCharacter::LockOnSwitch()
{
	if (CharacterStatComponent)
	{
		if (CharacterStatComponent->GetCharacterTargetingState() == ETargetingState::NonTargeting) // 락오프 -> 락온
		{
			LockOn();
		}
		else // 락온 -> 락오프																
		{
			CharacterStatComponent->SetCharacterTargetingState(ETargetingState::NonTargeting);
			LockOff();
		}
	}
}

void AIVPlayerCharacter::Interact()
{
	if (InteractionComponent)
	{
		InteractionComponent->SearchAndInteract();
	}
}

void AIVPlayerCharacter::UseQuickSlot_1()
{
	if (InventoryComponent)
	{
		InventoryComponent->UseItemFromSlot(EInventorySlotType::QuickSlot, 0);
	}
}

void AIVPlayerCharacter::UseQuickSlot_2()
{
	if (InventoryComponent)
	{
		InventoryComponent->UseItemFromSlot(EInventorySlotType::QuickSlot, 1);
	}
}

void AIVPlayerCharacter::UseQuickSlot_3()
{
	if (InventoryComponent)
	{
		InventoryComponent->UseItemFromSlot(EInventorySlotType::QuickSlot, 2);
	}
}

void AIVPlayerCharacter::UseQuickSlot_4()
{
	if (InventoryComponent)
	{
		InventoryComponent->UseItemFromSlot(EInventorySlotType::QuickSlot, 3);
	}
}



void AIVPlayerCharacter::LockOn()
{
	// 락온 대상 탐색을 위한 콜리전 설정
	TArray<FHitResult> HitResults;
	FVector Start = GetActorLocation();
	FVector End = Start; // 원점
	FQuat Rotation = FQuat::Identity;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(LockOnDistance);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->SweepMultiByObjectType(
		HitResults,
		Start,
		End,
		Rotation,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
		CollisionShape,
		QueryParams))
	{
		AActor* ClosestActor = nullptr;
		float ClosestDistance = LockOnDistance;

		// 락온 대상 중 가장 가까운 액터 선택
		for (const FHitResult& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor && HitActor->Implements<UIIVLockOnTargetMarker>()) // 락온 대상 조건 확인
			{
				if (IsTargetVisibleByLineTrace(HitActor)) // 시야에 있는지 확인
				{
					float Distance = FVector::Distance(Start, HitActor->GetActorLocation());
					if (Distance < ClosestDistance)
					{
						ClosestDistance = Distance;
						ClosestActor = HitActor;
					}
				}
			}
		}

		// 실질 락온 작업
		if (ClosestActor)
		{
			// 실제로 대상이 있어야 타겟팅 모드로 돌입
			CharacterStatComponent->SetCharacterTargetingState(ETargetingState::OnTargeting);

			// 플레이어 입력 및 거리에 따른 종료 조건 제어
			LockOnActor = ClosestActor;
			GetController()->SetIgnoreLookInput(true); // 락온 중 시점 입력 무시
			GetWorldTimerManager().SetTimer(LockOnCheckTimer, this, &AIVPlayerCharacter::CheckLockOnDistance, 0.1f, true);
		
			// 타겟 액터에 마킹 위젯 표시
			if (PlayerController)
			{
				PlayerController->ShowTargetMarker(ClosestActor);
			}

			// 모션 워핑 대상으로 지정
			if (MotionWarpingComponent)
			{
				MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(FName("Target"), ClosestActor->GetActorTransform());
			}

			// 회전제어
			GetCharacterMovement()->bOrientRotationToMovement = false; // 자동 회전 방지

			// 타겟팅 시 갑작스러운 캐릭터 회전을 방지하기 위해 조금 회전한 후 컨트롤러 회전 사용
			//bUseControllerRotationYaw = true;
		}
	}
}

void AIVPlayerCharacter::LockOff()
{
	LockOnActor = nullptr;
	GetController()->SetIgnoreLookInput(false); // 락온 해제 시 시점 입력 허용
	GetWorldTimerManager().ClearTimer(LockOnCheckTimer);

	// 타겟 액터에 마킹 위젯 해제
	if (PlayerController)
	{
		PlayerController->HideTargetMarker();
	}

	// 모션 워핑 대상 해제
	if (MotionWarpingComponent)
	{
		MotionWarpingComponent->RemoveWarpTarget(FName("Target"));
	}

	// 회전 제어
	GetCharacterMovement()->bOrientRotationToMovement = true; // 자동 회전 
	bUseControllerRotationYaw = false;
}

bool AIVPlayerCharacter::IsTargetVisibleByLineTrace(AActor* Target)
{
	if (!Target) return false;

	FVector TargetLocation = Target->GetActorLocation();
	FHitResult SightResult;
	FCollisionQueryParams SightParams;
	SightParams.AddIgnoredActor(this);
	SightParams.AddIgnoredActor(Target);

	// 타깃과 본인 사이가 가로막혀있다면 시야에 없는 것으로 판단
	bool bCanSee = !GetWorld()->LineTraceSingleByChannel(
		SightResult,
		GetActorLocation(),
		TargetLocation,
		ECC_Visibility, 
		SightParams
	);

	return bCanSee;
}

void AIVPlayerCharacter::CheckLockOnDistance()
{
	if (LockOnActor)
	{
		// 거리를 벗어나거나 시야에 없다면 락온 해제
		float Distance = FVector::Distance(GetActorLocation(), LockOnActor->GetActorLocation());
		if (Distance > LockOnDistance || !IsTargetVisibleByLineTrace(LockOnActor))
		{
			LockOnSwitch();
		}
	}
}

void AIVPlayerCharacter::MonsterDeath(AActor* DeadMonster)
{
	if (CharacterStatComponent && CharacterStatComponent->GetCharacterTargetingState() == ETargetingState::OnTargeting)
	{
		if (DeadMonster == LockOnActor)
		{
			LockOnSwitch();
		}
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
			// 다음 공격 입력은 가능하지만, 아직 몽타주 전체가 끝난 것은 아니므로 움직임 불가.
		}
		// 2회차 검사는 1회차 검사 여부와 관계 없이 항상 진행
		else
		{
			// 1회차를 패스하고 2회차 검사를 진행하는 경우(몽타주 종료 노티파이 호출되지 않은 경우)
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

void AIVPlayerCharacter::AttackCancel()
{
	// 공격을 시도했는데 무기가 없어서 공격이 취소된 경우
	if (CharacterStatComponent)
	{
		CharacterStatComponent->SetCharacterSpecialMoveState(ESpecialMovementState::None);
	}
}

void AIVPlayerCharacter::ResetComboEnd()
{
	bInComboAttack = false;
}

void AIVPlayerCharacter::SetDead()
{
	// 모든 몽타주 중지
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.0f);
	}

	// 무기 해제
	if (EquipComponent)
	{
		EquipComponent->DropWeapon();
	}

	// 인벤토리 비우기
	if (InventoryComponent)
	{
		InventoryComponent->ResetInventory();
	}

	// 래그돌 활성화
	Super::SetDead();
}

void AIVPlayerCharacter::SetAlive()
{
	Super::SetAlive();

	// 부활 이벤트 시 게임모드에게 플레이어 재생성을 요청
	AIVANGameMode* GameMode = Cast<AIVANGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->RespawnPlayer(this);
	}
}

void AIVPlayerCharacter::RespawnComplete()
{
	// 빙의가 될 때 까지 함수 재호출
	if (GetController() == nullptr)
	{
		FTimerHandle RespawnTimer;
		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, [this]()
			{
				RespawnComplete();
			}, 0.1f, false);
	}
	// 빙의 및 캐릭터 재생성이 완료되었음을 전달
	else
	{
		UGameInstance* GameInstance = GetWorld()->GetGameInstance();
		if (GameInstance)
		{
			UIVDeathEventSubsystem* DeathEventSubsystem = GameInstance->GetSubsystem<UIVDeathEventSubsystem>();
			if (DeathEventSubsystem)
			{
				DeathEventSubsystem->PlayerRespawnCompleteDelegate.Broadcast();
			}
		}
	}
	return;
}

float AIVPlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// 데미지 처리 -> 데미지를 받고 살아남은 경우에만 피격 리액션 진행
	if (CharacterStatComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser))
	{
		HitReactionComponent->ComputeHitAngle(Damage, DamageEvent, EventInstigator, DamageCauser);
		StartHitReaction();
	}
	// 그렇지 않은 경우는 스탯 컴포넌트에서 사망 처리가 진행된다.
	return 0.0f;
}


