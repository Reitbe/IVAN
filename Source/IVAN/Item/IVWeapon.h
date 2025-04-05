// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Item/IVItemBase.h"
#include "IVAN/Interface/IIVWeaponInterface.h"
#include "IVAN/Attack/IVAttackRange.h"
#include "IVWeapon.generated.h"

class UCapsuleComponent;
class UParticleSystem;
class USoundCue;
class UStaticMesh;

/**
 * 아이템을 상속받은 무기 클래스.
 * 공격에 사용되는 몽타주, 이펙트, 콤보 정보 등을 가지고 있다. 
 * 무기가 활성화 된 동안 충돌 판정 및 데미지 처리를 진행한다.
 */
UCLASS()
class IVAN_API AIVWeapon : public AIVItemBase, public IIIVWeaponInterface
{
	GENERATED_BODY()

// 기본
public:
	AIVWeapon();
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;


// 소유자 정보
private:
	/* 무기 소유자 접근용 */
	TObjectPtr<AController> OwnerController;

	/* 무기 소유자의 데미지 스텟 */
	FBaseDamageStat OwnerDamageStat;


// 무기 장착
public:
	void ApplyEquipSettings();

// 무기 상세 정보
protected:
	/* 콤보에 사용될 공격 몽타주 목록 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Detail")
	TArray<TObjectPtr<UAnimMontage>> ComboMontages;

	/* 무기 전환 몽타주 - 미구현 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Detail")
	TArray<TObjectPtr<UAnimMontage>> WeaponChangeMontage;

	/* 충돌 이펙트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Detail")
	TObjectPtr<UParticleSystem> HitEffect;

	/* 충돌 사운드 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Detail")
	TObjectPtr<USoundCue> HitSound;

	/* 무기의 최대 가능 콤보 수 -> 콤보 몽타주 수로 결정 */
	int32 MaxComboCount;


// 충돌 판정
public:
	/* 무기의 충돌 판정 활성화 및 진행 */ 
	void HitDetection();

	/* 이번 활성화 동안 충돌한 충돌 액터 목록 초기화 */
	void ClearHitActors();

protected:
	/*
	* 충돌 판정에 활용할 콜라이더 목록 
	* 실제 충돌 판정은 콜라이더의 위치와 방향, 크기를 기준으로 Trace를 진행한다.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Detail")
	TArray<UIVAttackRange*> HitColliders;

	/* 무기와 충돌한 액터 목록 */
	TArray<TObjectPtr<AActor>> HitActors;

	/* 충돌 대상에게 전달할 최종 데미지*/
	float TotalDamage;

	
// 파티클 및 사운드 
protected:
	/* 충돌 이펙트 재생 */
	void PlayHitEffect(FVector HitLocation, FVector_NetQuantizeNormal HitNormal);

	/* 충돌 사운드 재생 */
	void PlayHitSound(FVector HitLocation);


// IIIVWeaponInterface 인터페이스 - AttackComponent 연결용 
public:
	virtual void SetOwnerController(AController* NewOwnerController) override;
	virtual void SetOwnerDamageStat(FBaseDamageStat NewDamageStat) override;
	virtual void SetOwnerAttackRanges(const TArray<UIVAttackRange*>& AttackRanges) override;
	virtual int32 GetMaxComboCount() const override { return MaxComboCount; };
	virtual UAnimMontage* GetComboMontage(int32 ComboIndex) const override;

};
