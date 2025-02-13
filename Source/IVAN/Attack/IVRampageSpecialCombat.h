// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "IVAN/Interface/IIVAICharacterSpecialCombat.h"
#include "IVRampageSpecialCombat.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IVAN_API UIVRampageSpecialCombat : public UActorComponent //, public IIIVAICharacterSpecialCombat
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UIVRampageSpecialCombat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// 인터페이스 구현
public:
	///* IIIVAICharacterSpecialCombat 인터페이스 구현 */
	//virtual void SpecialMove(AActor* TargetActor) const override;
	//virtual void SpecialAttack(AActor* TargetActor) const override;

};
