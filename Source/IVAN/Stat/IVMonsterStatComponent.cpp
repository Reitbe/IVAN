// Fill out your copyright notice in the Description page of Project Settings.


#include "IVMonsterStatComponent.h"

UIVMonsterStatComponent::UIVMonsterStatComponent()
{
	MonsterState = EMonsterState::Idle;
}

void UIVMonsterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UIVMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// 사망 이벤트 바인딩
	OnMonsterDeathLocalEvent.AddUObject(this, &UIVMonsterStatComponent::SetDead);
}

void UIVMonsterStatComponent::SetDead()
{
	Super::SetDead();
	EMonsterState NewState = EMonsterState::Dead;
}

void UIVMonsterStatComponent::SetAlive()
{
	Super::SetAlive();
	EMonsterState NewState = EMonsterState::Idle;
}

void UIVMonsterStatComponent::AttachStat(const FBaseStat& OtherStat)
{
	Super::AttachStat(OtherStat);
	OnHpChanged.ExecuteIfBound(BaseStat.MaxHP, BaseStat.CurrentHP);
}

void UIVMonsterStatComponent::DetachStat(const FBaseStat& OtherStat)
{
	Super::DetachStat(OtherStat);
	OnHpChanged.ExecuteIfBound(BaseStat.MaxHP, BaseStat.CurrentHP);
}

void UIVMonsterStatComponent::SetBaseStat(const FBaseStat& NewBaseStat)
{
	Super::SetBaseStat(NewBaseStat);
	OnHpChanged.ExecuteIfBound(BaseStat.MaxHP, BaseStat.CurrentHP);
}

bool UIVMonsterStatComponent::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	DetachStat(FBaseStat(0.0f, Damage, 0.0f, 0.0f)); // 체력 감소 처리

	// 체력이 0 이하로 떨어지면 사망 처리
	if (BaseStat.CurrentHP <= 0)
	{
		BaseStat.CurrentHP = FMath::Clamp(BaseStat.CurrentHP, 0.0f, BaseStat.MaxHP);

		// 글로벌 이벤트 서브시스템을 통해 사망 이벤트 전달
		if (LifeEventSubsystem)
		{
			LifeEventSubsystem->MonsterDeathEventDelegate.Broadcast(GetOwner());
			OnMonsterDeathLocalEvent.Broadcast();
		}
		return false;
	}
	return true;
}
