
// Fill out your copyright notice in the Description page of Project Settings.


#include "IVAttackRange.h"

UIVAttackRange::UIVAttackRange(const FObjectInitializer& ObjectInitializer)
{
	// 콜라이더의 충돌 설정
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
