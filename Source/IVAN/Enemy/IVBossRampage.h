// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Enemy/IVBossEnemy.h"
#include "IVBossRampage.generated.h"

class UAnimMontage;

/**
 * 
 */
UCLASS()
class IVAN_API AIVBossRampage : public AIVBossEnemy
{
	GENERATED_BODY()

public:
	AIVBossRampage();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


	// 램페이지 공격 범위 - 양손에 각각 하나씩
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack")
	TObjectPtr<UIVAttackRange> RightHandAttackRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack")
	TObjectPtr<UIVAttackRange> LeftHandAttackRange;


// 램페이지 특수 동작
public:
	virtual void SpecialMove(AActor* TargetActor) override;
	virtual void SpecialAttack(AActor* TargetActor) override;

	void OnSpecialMoveEnd(AActor* TargetActor);
	void OnSpecialAttackEnd(AActor* TargetActor);
	

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpecialAttack")
	TObjectPtr<UAnimMontage> SpecialMoveMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpecialAttack")
	TObjectPtr<UAnimMontage> SpecialAttackMontage;

};
