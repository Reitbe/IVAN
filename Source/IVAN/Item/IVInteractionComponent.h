// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IVAN/Interface/IIVInteractableInterface.h"
#include "IVInteractionComponent.generated.h"

/*
* 캐릭터에 부착되어 상호작용을 담당하는 컴포넌트.
* 상호작용 대상은 IIIVInteractableInterface를 구현한 액터여야 한다.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IVAN_API UIVInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

// 기본
public:	
	UIVInteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;


// 상호작용
public:
	/* 범위 내의 상호작용 가능 액터를 찾아서 가장 가까운 액터와 상호작용한다 */
	void SearchAndInteract();

	/* 상호작용 대상이 범위 내에 있으며 시야에 가로막히지 않는지 여부 판단 */
	bool IsTargetInteractable(AActor* Target);

private:
	/* 상호작용 거리 */
	float InteractionDistance;
};
