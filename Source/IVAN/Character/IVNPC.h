// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IVAN/Character/IVCharacterBase.h"
#include "IVAN/Interface/IIVInteractableInterface.h"
#include "IVNPC.generated.h"

class UIVDialogueComponent;
class USphereComponent;
class UWidgetComponent;

/**
 * 대화 상호작용이 가능한 NPC 클래스
 */
UCLASS()
class IVAN_API AIVNPC : public AIVCharacterBase, public IIIVInteractableInterface
{
	GENERATED_BODY()

// 기본
public:
	AIVNPC();

protected:
	virtual void BeginPlay() override;


// 대화 관련
public:
	/* 대화 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TObjectPtr<UIVDialogueComponent> DialogueComponent;

private:
	/* 대화 중인지 확인하기 위한 변수 */
	uint8 bIsInDialogue : 1; 


// 플레이어와 상호작용
public:
	/* 상호작용 범위 감지용 콜리전 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip")
	TObjectPtr<USphereComponent> InteractionCollision;

	/* 상호작용 활성화 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip")
	uint8 bIsInteractable : 1;

	/* 콜리전 범위 진입 시 */
	UFUNCTION()
	void OnInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* 콜리전 범위 벗어날 시 */
	UFUNCTION()
	void OnInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


// 플레이어 상호작용 위젯
public:
	/* 상호작용 위젯 표시용 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UWidgetComponent> WidgetComponent;

	/* 플레이어가 상호작용 범위 내로 들어왔을 때 표시할 위젯 */
	TObjectPtr<UUserWidget> TalkInteractionWidget;


// 인터페이스
public:
	/* IIIVInteractableInterface 구현 */
	virtual void Interact(AActor* InteractingActor) override;
	virtual void SetInteractable(bool bNewInteractable) override;
	virtual bool IsInteractable() const override { return bIsInteractable; };
	virtual void ShowInteractionUI() override;
	virtual void HideInteractionUI() override;
	
};
