// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IVCharacterBase.generated.h"

class USkeletalMeshComponent;

UCLASS()
class IVAN_API AIVCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AIVCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// 모듈형 의상 시스템
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
