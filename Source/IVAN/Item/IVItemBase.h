// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IVAN/Enums/IVGenericItemEnums.h"
#include "IVAN/IVGenericStructs.h"
#include "IVAN/Interface/IIVInteractableInterface.h"
#include "IVItemBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UWidgetComponent;

/*
* 아이템의 기본이 되는 클래스.
* 모든 아이템이 가지는 기본 상호작용 및 정보를 담당한다.
*/
UCLASS()
class IVAN_API AIVItemBase : public AActor, public IIIVInteractableInterface
{
	GENERATED_BODY()
	
// 기본
public:	
	AIVItemBase();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;


// 아이템 세부사항
public:
	/* 레벨에 아이템을 배치하는지 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	uint8 bIsPlacedInWorld : 1;

	/* 데이터베이스에서 가져온 아이템 세부 정보 - BP에서도 ID만큼은 제대로 작성해야한다.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemBaseInfo ItemInfo;

	/* 아이템 메쉬 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<UStaticMeshComponent> ItemMeshComponent;

	/* 위치 종합용 씬 컴포넌트 */
	TObjectPtr<USceneComponent> SceneComponent;


// 아이템 상호작용
public:
	/* 아이템 상호작용 범위 감지용 콜리전 */ 
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


// 아이템 상호작용 위젯
public:
	/* 상호작용 범위 내로 들어왔을 때 표시할 위젯 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UWidgetComponent> WidgetComponent;

	/* 아이템 상호작용 - 줍기 위젯 */
	TObjectPtr<UUserWidget> PickUpWidget;


// 아이템 공통 동작
public:
	/* 특정 아이템 정보로 액터를 초기화 */
	void InitializeItem(const FItemBaseInfo& InItemInfo, int32 ItemCount);

	/* 아이템을 레벨 내 액터로 스폰*/
	void DropItem();


// 인터페이스
public:
	/* IIIVInteractableInterface 구현 */
	virtual void Interact(AActor* InteractingActor) override;
	virtual void SetInteractable(bool bNewInteractable) override;
	virtual bool IsInteractable() const override { return bIsInteractable; };
	virtual void ShowInteractionUI() override;
	virtual void HideInteractionUI() override;
};
