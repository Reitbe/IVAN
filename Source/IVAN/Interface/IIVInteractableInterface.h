// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IIVInteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 캐릭터-외부 물체 상호작용을 위한 인터페이스. 
 * 외부 물체에 부착되어 동작 및 UI를 제어한다.
 */
class IVAN_API IIIVInteractableInterface
{
	GENERATED_BODY()

public:
	/* 구체적인 상호작용 동작 */
	virtual void Interact(AActor* InteractingActor) = 0;

	/* 상호작용 가능 여부 설정 */
	virtual void SetInteractable(bool bNewInteractable) = 0;
	virtual bool IsInteractable() const = 0;

	/* 상호작용 UI 효현 여부 */
	virtual void ShowInteractionUI() = 0;
	virtual void HideInteractionUI() = 0;
};
