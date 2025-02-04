﻿// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IVANGameMode.generated.h"

UCLASS(minimalapi)
class AIVANGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIVANGameMode();
	virtual void BeginPlay() override;

// 사망처리
public:
	void OnPlayerDeath();
	void OnPlayerAlive();
};



