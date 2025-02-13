// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "IVAttackRange.generated.h"

/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class IVAN_API UIVAttackRange : public UCapsuleComponent
{
	GENERATED_BODY()

public:
	UIVAttackRange(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
