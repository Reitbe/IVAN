// Fill out your copyright notice in the Description page of Project Settings.


#include "IVCharacterStatComponent.h"
#include "IVAN/Interface/IIVCharacterComponentProvider.h"
#include "IVAN/GameSystem/IVDeathEventSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UIVCharacterStatComponent::UIVCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	MovementState = EMovementState::Idle;
	GaitState = EGaitState::Walk;
	JumpState = EJumpState::OnGround;
	TargetingState = ETargetingState::NonTargeting;

	BaseStat.MaxHP = 100.0f;
	BaseStat.CurrentHP = BaseStat.MaxHP;
}

void UIVCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// 캐릭터 움직임 정보 획득용 컴포넌트
	AActor* Owner = GetOwner();
	if (Owner != nullptr && Owner->Implements<UIIVCharacterComponentProvider>())
	{
		IIIVCharacterComponentProvider* Provider = Cast<IIIVCharacterComponentProvider>(Owner);
		CharacterMovementComponent = Provider->GetCharacterMovementComponent();
	}
}

void UIVCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 캐릭터 움직임 정보 갱신
	if (CharacterMovementComponent)
	{
		JumpState = (CharacterMovementComponent->IsFalling()) ? EJumpState::InAir : EJumpState::OnGround; // 점프 상태 갱신	
		MovementState = (CharacterMovementComponent->Velocity.Size2D() > 0.0f) ? EMovementState::Move : EMovementState::Idle; // 움직임 상태 갱신
		// 걸음걸이 상태 -> 플레이어 캐릭터에서 지정
		// 타겟팅 상태 -> 플레이어 캐릭터에서 지정
	}
}

float UIVCharacterStatComponent::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("나 맞았어용"));

	// 체력 감소 처리
	DetachStat(FBaseStat(0.0f, Damage, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));

	// 체력이 0 이하로 떨어지면 사망 처리
	if (BaseStat.CurrentHP <= 0)
	{
		BaseStat.CurrentHP = FMath::Clamp(BaseStat.CurrentHP, 0.0f, BaseStat.MaxHP);

		// 글로벌 이벤트 서브시스템을 통해 사망 이벤트 전달
		UGameInstance* GameInstance = GetWorld()->GetGameInstance();
		if (GameInstance)
		{
			UIVDeathEventSubsystem* DeathEventSubsystem = GameInstance->GetSubsystem<UIVDeathEventSubsystem>();
			if (DeathEventSubsystem)
			{
				DeathEventSubsystem->PlayerDeathEventDelegate.Broadcast();
			}
		}
	}
	return 0.0f;
}

void UIVCharacterStatComponent::AttachStat(const FBaseStat& OtherStat)
{
	BaseStat = BaseStat + OtherStat;
	OnBaseStatChanged.ExecuteIfBound(BaseStat);
}

void UIVCharacterStatComponent::AttachStat(const FBaseDamageStat& OtherStat)
{
	BaseDamageStat = BaseDamageStat + OtherStat;
}

void UIVCharacterStatComponent::DetachStat(const FBaseStat& OtherStat)
{
	BaseStat = BaseStat - OtherStat;
	OnBaseStatChanged.ExecuteIfBound(BaseStat);
}

void UIVCharacterStatComponent::DetachStat(const FBaseDamageStat& OtherStat)
{
	BaseDamageStat = BaseDamageStat - OtherStat;
}

void UIVCharacterStatComponent::SetBaseStat(const FBaseStat& NewBaseStat)
{
	BaseStat = NewBaseStat;
	OnBaseStatChanged.ExecuteIfBound(BaseStat);
}

void UIVCharacterStatComponent::SetBaseDamageStat(const FBaseDamageStat& NewBaseDamageStat)
{
	BaseDamageStat = NewBaseDamageStat;
}

