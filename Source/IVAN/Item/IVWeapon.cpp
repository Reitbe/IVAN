// Fill out your copyright notice in the Description page of Project Settings.


#include "IVWeapon.h"
#include "IVAN/IVGenericStructs.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AIVWeapon::AIVWeapon()
{
	ItemType = EItemType::Weapon;

	// 무기 외형 스테틱 메시
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(WeaponMesh);

	// 충돌 탐지용 콜라이더 생성
	//HitCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollider"));
	//HitCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollider->SetupAttachment(RootComponent);

	TotalDamage = 0.0f;
	MaxComboCount = 0;
}

void AIVWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 콤보 몽타주 수로 최대 콤보 수 결정
	MaxComboCount = ComboMontages.Num();

	// 가지고 있는 콜라이더들 싸그리 긁어오기
	GetComponents<UIVAttackRange>(HitColliders);
}

void AIVWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AIVWeapon::DropWeapon()
{
	// 무기 물리	시뮬레이션 활성화
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	WeaponMesh->SetSimulatePhysics(true);
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

void AIVWeapon::SetOwnerAttackRanges(const TArray<UIVAttackRange*>& AttackRanges)
{
	HitColliders.Append(AttackRanges);
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
	// 보유한 모든 콜라이더에 대하여 Trace 진행
	for (UIVAttackRange* HitCollider : HitColliders)
	{
		// 콜라이더의 위치와 방향을 기준으로 Trace
		TArray<FHitResult> HitResults;
		FVector Start = HitCollider->GetComponentLocation(); // 종료 지점 동일
		FQuat Rotation = HitCollider->GetComponentQuat();
		FCollisionShape CollisionShape = HitCollider->GetCollisionShape();
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		// Trace 진행
		if (GetWorld()->SweepMultiByObjectType(
			HitResults,
			Start,
			Start,
			Rotation,
			FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
			CollisionShape,
			CollisionParams))
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

		// 궤적 확인용 디버그 콜라이더 생성
		DrawDebugCapsule(
			GetWorld(),
			Start,
			CollisionShape.GetCapsuleHalfHeight(),
			CollisionShape.GetCapsuleRadius(),
			Rotation, FColor::Red,
			false,
			1.0f);
	}
}

void AIVWeapon::ClearHitActors()
{
	// 충돌한 액터 목록 초기화
	HitActors.Empty();
}
