// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IIVInventoryComponentProvider.generated.h"

class UIVInventoryComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIVInventoryComponentProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 위젯에게 캐릭터의 인벤토리 컴포넌트를 제공하기 위한 인터페이스
 */
class IVAN_API IIIVInventoryComponentProvider
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual class UIVInventoryComponent* GetInventoryComponent() const = 0;
};
