// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IVDialogueComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IVAN_API UIVDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

// 기본
public:	
	UIVDialogueComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
// 대화용 정보
public:
	void StartDialogue();

protected:
	/* 대화용 NPC ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName NPCID;
};
