// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IVAN/Interface/IIVInteractableInterface.h"
#include "IVSavePoint.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UWidgetComponent;

/*
* 게임 진행 상황 저장을 위한 세이브 포인트. 
* 플레이어 사망 시 해당 위치에서 부활한다.
*/

UCLASS()
class IVAN_API AIVSavePoint : public AActor, public IIIVInteractableInterface
{
	GENERATED_BODY()
	
// 기본
public:	
	AIVSavePoint();

protected:
	virtual void BeginPlay() override;


// 세이브 포인트 세부사항
public:
	/* 메쉬 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	/* 위치 종합용 씬 컴포넌트 */
	TObjectPtr<USceneComponent> SceneComponent;


//  상호작용
public:
	/* 상호작용 범위 감지용 콜리전 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip")
	TObjectPtr<USphereComponent> InteractionCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip")
	uint8 bIsInteractable : 1;

	/* 콜리전 범위 진입 시 */
	UFUNCTION()
	void OnInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* 콜리전 범위 벗어날 시 */
	UFUNCTION()
	void OnInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


// 상호작용 위젯
public:
	/* 상호작용 범위 내로 들어왔을 때 표시할 위젯 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UWidgetComponent> WidgetComponent;

	/* 장착할 위젯 */
	TObjectPtr<UUserWidget> InteractionWidget;


// 인터페이스
public:
	/* IIIVInteractableInterface 구현 */
	virtual void Interact(AActor* InteractingActor) override;
	virtual void SetInteractable(bool bNewInteractable) override;
	virtual bool IsInteractable() const override { return bIsInteractable; };
	virtual void ShowInteractionUI() override;
	virtual void HideInteractionUI() override;
};
