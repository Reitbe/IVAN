// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBaseStatComponent.h"

UIVBaseStatComponent::UIVBaseStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 기본 스탯 초기화
	BaseStat.MaxHP = 100.0f;
	BaseStat.CurrentHP = BaseStat.MaxHP;
	BaseStat.MaxStamina = 100.0f;
	BaseStat.CurrentStamina = BaseStat.MaxStamina;

	// 기본 데미지 스탯 초기화
	BaseDamageStat.BaseDamage = 10.0f;
	BaseDamageStat.AdditionalDamage = 0.0f;
	BaseDamageStat.DamageReduction = 0.0f;
}

void UIVBaseStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UIVBaseStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// 글로벌 이벤트 서브시스템 접근
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
	{
		LifeEventSubsystem = GameInstance->GetSubsystem<UIVDeathEventSubsystem>();
	}
	
}

void UIVBaseStatComponent::AttachStat(const FBaseStat& OtherStat)
{
	BaseStat = BaseStat + OtherStat;
	OnBaseStatChanged.ExecuteIfBound(BaseStat);
}

void UIVBaseStatComponent::AttachStat(const FBaseDamageStat& OtherStat)
{
	BaseDamageStat = BaseDamageStat + OtherStat;
}

void UIVBaseStatComponent::DetachStat(const FBaseStat& OtherStat)
{
	BaseStat = BaseStat - OtherStat;
	OnBaseStatChanged.ExecuteIfBound(BaseStat);
}

void UIVBaseStatComponent::DetachStat(const FBaseDamageStat& OtherStat)
{
	BaseDamageStat = BaseDamageStat - OtherStat;
}

void UIVBaseStatComponent::SetBaseStat(const FBaseStat& NewBaseStat)
{
	BaseStat = NewBaseStat;
	OnBaseStatChanged.ExecuteIfBound(BaseStat);
}

void UIVBaseStatComponent::SetBaseDamageStat(const FBaseDamageStat& NewBaseDamageStat)
{
	BaseDamageStat = NewBaseDamageStat;
}

bool UIVBaseStatComponent::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return true;
}

void UIVBaseStatComponent::SetDead()
{
}

void UIVBaseStatComponent::SetAlive()
{
}


