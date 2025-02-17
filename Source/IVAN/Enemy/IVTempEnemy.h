// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IVAN/Interface/IIVLockOnTargetMarker.h"
#include "IVTempEnemy.generated.h"

class UWidgetComponent;
class UIVBaseStatBar;
class UBoxComponent;

/*
* 캐릭터의 공격 및 피격 테스트를 위한 임시 몬스터
*/

UCLASS()
class IVAN_API AIVTempEnemy : public AActor, public IIIVLockOnTargetMarker
{
	GENERATED_BODY()

// 기본
public:	
	AIVTempEnemy();
	virtual void Tick(float DeltaTime) override;

	/* 루트 컴포넌트 설정용 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> BoxComponent;

protected:
	virtual void BeginPlay() override;


// 피격 처리
public:	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

// 체력 스탯 및 위젯
public:
	/* 체력 위젯 장착용 위젯 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UWidgetComponent> WidgetComponent;

	/* 체력 바 위젯 */
	TObjectPtr<UIVBaseStatBar> HealthBar;


private:
	float MaxHealth;
	float CurrentHealth;

};
