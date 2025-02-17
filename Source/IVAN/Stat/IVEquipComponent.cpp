// Fill out your copyright notice in the Description page of Project Settings.


#include "IVEquipComponent.h"
#include "IVAN/Item/IVWeapon.h"
#include "IVAN/Interface/IIVEquipInterface.h"

UIVEquipComponent::UIVEquipComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UIVEquipComponent::BeginPlay()
{
	Super::BeginPlay();
	EquipWeapon();
}

void UIVEquipComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UIVEquipComponent::EquipWeapon()
{
	// 무기 클래스로부터 무기 인스턴스 생성
	UWorld* World = GetWorld();
	if (World)
	{
		WeaponInstance = World->SpawnActor<AIVWeapon>(WeaponClass, FVector().ZeroVector, FRotator().ZeroRotator);
	}

	/*
	* 생성된 무기 오너에게 전달하며 부착을 요청한다.
	* 부착 위치는 무기 클래스에 저장되어있다. 
	*/
	if (WeaponInstance)
	{
		IIIVEquipInterface* EquipOwner = Cast<IIIVEquipInterface>(GetOwner());
		if (EquipOwner)
		{
			EquipOwner->EquipByInstance(WeaponInstance);
		}
	}
}

void UIVEquipComponent::UnequipWeapon()
{
	// 무기 물리	시뮬레이션 활성화
	if (WeaponInstance)
	{
		WeaponInstance->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		WeaponInstance->DropWeapon();
	}
}

void UIVEquipComponent::ChangeWeapon(TSubclassOf<AIVWeapon> NewWeaponClass)
{
}

void UIVEquipComponent::EquipArmor()
{
}

void UIVEquipComponent::UnequipArmor()
{
}

void UIVEquipComponent::ChangeArmor(TSubclassOf<AIVArmor> NewArmorClass)
{
}

