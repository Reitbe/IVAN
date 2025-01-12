// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "IVAnimBase.generated.h"

/**
 * 애님 인스턴스 공통 요소 정의용 클래스.
 * 아직 자식 클래스를 하나밖에 만들지 않아서 빈 클래스로 남겨둔다.
 */

UCLASS()
class IVAN_API UIVAnimBase : public UAnimInstance
{
	GENERATED_BODY()
	
};
