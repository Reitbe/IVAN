// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IVHitReactionComponent.generated.h"

class UAnimInstance;

/*
* 오너의 피격 리액션을 담당하는 컴포넌트
* 방향에 따른 피격 애니메이션, 래그돌 유무 등을 관리한다.
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IVAN_API UIVHitReactionComponent : public UActorComponent
{
	GENERATED_BODY()

// 기본
public:	
	UIVHitReactionComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


// 피격 각도 및 적합한 애니메이션 판단
public:
	/* 피격 정보와 오너의 방향 정보를 통해 공격이 들어온 방향을 계산한다 */
	void ComputeHitAngle(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

private:
	/* 앞서 계산한 피격 각도를 사용해 방향에 따른 피격 애니메이션을 재생한다 */
	void PlayHitReactionMontage(float Angle);


// 애니메이션
public:
	/* 피격 애니메이션을 재생할 대상의 AnimInstance 설정 */
	void SetAnimInstance(TObjectPtr<UAnimInstance> NewAnimInstance) { AnimInstance = NewAnimInstance; };

protected:
	/* 피격 대상(오너)의 AnimInstance */
	TObjectPtr<UAnimInstance> AnimInstance;


// 피격 리액션 몽타주
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction") // 전방 피격
	TObjectPtr<UAnimMontage> FrontHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction") // 후방 피격
	TObjectPtr<UAnimMontage> BackHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction") // 왼쪽 피격
	TObjectPtr<UAnimMontage> LeftHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction") // 오른쪽 피격
	TObjectPtr<UAnimMontage> RightHitMontage;

// 설정 관련 변수
protected:
	/* 피격 리액션 사용 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaction Option")
	uint8 bIsUsingHitReaction : 1;

	/* 사망 시 래그돌 사용 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaction Option")
	uint8 bIsUsingRagdoll : 1;
};
