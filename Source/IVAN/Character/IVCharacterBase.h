// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IVCharacterBase.generated.h"

class UIVEquipComponent;
class USkeletalMeshComponent;

/*
* 플레이어, 몬스터, NPC 등 캐릭터의 기본 클래스
*/

UCLASS()
class IVAN_API AIVCharacterBase : public ACharacter
{
	GENERATED_BODY()

// 기본
public:
	AIVCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


// 사망 처리
protected:
	virtual void SetDead();
	virtual void SetAlive();


// 장비 관련(모듈형 의상)
public:
	/* 캐릭터 장비 관리 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip")
	TObjectPtr<UIVEquipComponent> EquipComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularSkin", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> HelmMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularSkin", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> LegsMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularSkin", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ShoulderArmourMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularSkin", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FeetMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularSkin", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> HeadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularSkin", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> HandsMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularSkin", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> NecklaceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularSkin", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BeltMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularSkin", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> CloakMesh;

};
