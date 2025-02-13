// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "IVBossTriggerBox.generated.h"

class AIVPlayerCharacter;
class AIVBossEnemy;

/**
 * 트리거 범위에 들어갔을 때, 보스몬스터의 HUD 표시 및 숨김을 담당하는 클래스
 */
UCLASS()
class IVAN_API AIVBossTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	AIVBossTriggerBox();

protected:
	virtual void BeginPlay() override;


// 보스 및 플레이어 판별
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ActorInfo")
	TSoftObjectPtr<AIVPlayerCharacter> PlayerCharacter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ActorInfo")
	TSoftObjectPtr<AIVBossEnemy> BossEnemy;


// 콜라이더 관련
protected:
	/* OnComponentBeginOverlap 시 호출되는 함수 */
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* OnComponentEndOverlap 시 호출되는 함수 */
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
