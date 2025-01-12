// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IVCharacterBase.generated.h"

class USkeletalMeshComponent;
class UIVCharacterStatComponent;

/*
* �پ��� ĳ���͵��� �⺻�� �Ǵ� Ŭ����.
* ����� �ǻ� �ý����� ���� �и��Ͽ� ������ Ŭ������ ������ ����.
*/

UCLASS()
class IVAN_API AIVCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AIVCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// ���� �ý���
	TObjectPtr<UIVCharacterStatComponent> CharacterStatComponent;


// ����� �ǻ� �ý���(���� �и�)
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
