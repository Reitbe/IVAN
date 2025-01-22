// Fill out your copyright notice in the Description page of Project Settings.


#include "IVWeapon.h"
#include "IVAN/IVGenericStructs.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AIVWeapon::AIVWeapon()
{
	ItemType = EItemType::Weapon;

	TotalDamage = 0.0f;
}

void AIVWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 콤보 몽타주 수로 최대 콤보 수 결정
	MaxComboCount = ComboMontages.Num();

	// 모든 콜라이더 비활성화 및 루트 컴포넌트에 부착
	for (auto Collider : HitColliders)
	{
		Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Collider->SetupAttachment(RootComponent);
	}
}

void AIVWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AIVWeapon::SetOwnerController(AController* NewOwnerController)
{
	OwnerController = NewOwnerController;
	SetOwner(NewOwnerController->GetPawn());
}

void AIVWeapon::SetOwnerDamageStat(FBaseDamageStat NewDamageStat)
{
	OwnerDamageStat = NewDamageStat;

	// 최종 데미지 = 무기 소유자 기본&추가 데미지 + 무기의 기본&추가 데미지
	TotalDamage = OwnerDamageStat.BaseDamage + OwnerDamageStat.AdditionalDamage 
		+ DamageStat.BaseDamage + DamageStat.AdditionalDamage;
}

UAnimMontage* AIVWeapon::GetComboMontage(int32 ComboIndex) const
{
	if (ComboMontages.IsValidIndex(ComboIndex))
	{
		return ComboMontages[ComboIndex].Get();
	}
	return nullptr;
}

void AIVWeapon::HitDetection()
{
	// 모든 콜라이더를 순회하며 이번에 사용할 충돌 Trace 설정
	for (auto& Collider : HitColliders)
	{
		// 콜라이더의 위치와 방향을 기준으로 Trace
		FVector Start = Collider->GetComponentLocation();
		FQuat Rotation = Collider->GetComponentQuat();
		FCollisionShape CollisionShape = Collider->GetCollisionShape();

		// Trace를 통해 충돌한 액터들을 저장
		TArray<FHitResult> HitResults;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		// Trace 진행
		if (GetWorld()->SweepMultiByObjectType(
			HitResults,
			Start,
			Start,
			Rotation,
			FCollisionObjectQueryParams::AllDynamicObjects,
			CollisionShape))
		{
			// 이번 Trace에서 충돌한 액터들에게 데미지 적용
			for (const FHitResult& HitResult : HitResults)
			{
				if (!HitActors.Contains(HitResult.GetActor()) && HitResult.GetActor() != Owner)
				{
					UGameplayStatics::ApplyPointDamage(
						HitResult.GetActor(),
						TotalDamage,
						HitResult.Normal,
						HitResult,
						OwnerController, 
						this,
						UDamageType::StaticClass());
					HitActors.Add(HitResult.GetActor());
				}
			}
		}
	}
}

void AIVWeapon::ClearHitActors()
{
	// 충돌한 액터 목록 초기화
	HitActors.Empty();
}
