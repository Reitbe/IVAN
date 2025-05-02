// Fill out your copyright notice in the Description page of Project Settings.


#include "IVCharacterStatComponent.h"
#include "IVAN/Interface/IIVCharacterComponentProvider.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UIVCharacterStatComponent::UIVCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	// 초기 동작 상태 설정
	MovementState = EMovementState::Idle;
	GaitState = EGaitState::Walk;
	JumpState = EJumpState::OnGround;
	TargetingState = ETargetingState::NonTargeting;
	SpecialMovementState = ESpecialMovementState::None;
	LifeState = ELifeState::Alive;
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

	// 글로벌 이벤트 서브시스템을 통해 사망 및 부활 이벤트 제어
	if (LifeEventSubsystem)
	{
		LifeEventSubsystem->PlayerDeathEventDelegate.AddUObject(this, &UIVCharacterStatComponent::SetDead);
		LifeEventSubsystem->PlayerRespawnEventDelegate.AddUObject(this, &UIVCharacterStatComponent::SetAlive);
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

float UIVCharacterStatComponent::GetStatValue(EStatState StatType)
{
	switch (StatType)
	{
	case EStatState::MaxHP:
		return BaseStat.MaxHP;

	case EStatState::CurrentHP:
		return BaseStat.CurrentHP;
	}

	return 0.0f;
}

bool UIVCharacterStatComponent::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	DetachStat(FBaseStat(0.0f, Damage, 0.0f, 0.0f)); // 체력 감소 처리

	// 체력이 0 이하로 떨어지면 사망 처리
	if (BaseStat.CurrentHP <= 0)
	{
		BaseStat.CurrentHP = FMath::Clamp(BaseStat.CurrentHP, 0.0f, BaseStat.MaxHP);

		// 글로벌 이벤트 서브시스템을 통해 사망 이벤트 전달
		if(LifeEventSubsystem)
		{
			LifeEventSubsystem->PlayerDeathEventDelegate.Broadcast();
		}
		return false;
	}
	return true;
}

void UIVCharacterStatComponent::UseConsumableItem(FBaseStat& ItemBaseStat, FBaseDamageStat& ItemDamageStat)
{
	AttachStat(ItemBaseStat);
	AttachStat(ItemDamageStat);
}

void UIVCharacterStatComponent::EquipItem(FBaseStat& ItemBaseStat, FBaseDamageStat& ItemDamageStat)
{
	AttachStat(ItemBaseStat);
	AttachStat(ItemDamageStat);
}

void UIVCharacterStatComponent::UnequipItem(FBaseStat& ItemBaseStat, FBaseDamageStat& ItemDamageStat)
{
	DetachStat(ItemBaseStat);
	DetachStat(ItemDamageStat);
}

void UIVCharacterStatComponent::SetDead()
{
	Super::SetDead();

	LifeState = ELifeState::Dead;

	// 2초 후 부활 브로드캐스트
	//FTimerHandle RespawnTimer;
	//GetWorld()->GetTimerManager().SetTimer(RespawnTimer, [this]()
	//	{
	//		if (LifeEventSubsystem)
	//		{
	//			LifeEventSubsystem->PlayerRespawnEventDelegate.Broadcast();
	//		}
	//	}, 2.0f, false);
}

void UIVCharacterStatComponent::SetAlive()
{
	Super::SetAlive();

	//GaitState = EGaitState::Walk; // 캐릭터 속도 초기화와 함께 GaitState도 초기화 진행
	//TargetingState = ETargetingState::NonTargeting; // 캐릭터와 함께 TargetingState도 초기화 진행
	MovementState = EMovementState::Idle;
	JumpState = EJumpState::OnGround;
	SpecialMovementState = ESpecialMovementState::None;
	LifeState = ELifeState::Alive;

	SetBaseStat(FBaseStat(BaseStat.MaxHP, BaseStat.MaxHP, BaseStat.MaxStamina, BaseStat.MaxStamina)); // 체력 회복
}

