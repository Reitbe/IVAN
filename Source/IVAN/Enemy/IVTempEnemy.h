// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IVTempEnemy.generated.h"

class UWidgetComponent;
class UIVBaseStatBar;
class UBoxComponent;

/*
* 캐릭터의 공격 및 피격 테스트를 위한 임시 몬스터
*/

UCLASS()
class IVAN_API AIVTempEnemy : public AActor
{
	GENERATED_BODY()

// 기본
public:	
	AIVTempEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

// 피격
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

// 시각 표현용
public:
	/* 루트 컴포넌트 설정용 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> BoxComponent;
public:
	/* 체력 위젯 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UWidgetComponent> WidgetComponent;

	TObjectPtr<UIVBaseStatBar> HealthBar;

// 임시 스탯
private:
	float MaxHealth;
	float CurrentHealth;

};
