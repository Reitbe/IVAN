// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IVDialogueComponent.generated.h"

/*
* 플레이어와 대화할 상대에게 부착되는 대화용 컴포넌트.
* 독백의 경우에는 트리거 액터에 부착하여 사용 가능하다. 
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IVAN_API UIVDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

// 기본
public:	
	UIVDialogueComponent();

protected:
	virtual void BeginPlay() override;

// 대화용 정보
public:
	/* 대화를 시작한다 */
	void StartDialogue();

protected:
	/* 가능한 대화 목록에 접근하기 위한 NPC ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName NPCID;
};
