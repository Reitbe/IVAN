// Fill out your copyright notice in the Description page of Project Settings.


#include "IVCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AIVCharacterBase::AIVCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 캐릭터와 컨트롤러의 회전을 분리
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false; 
	bUseControllerRotationRoll = false;

	// 움직임 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 0.2f;

	// 모듈형 의상 시스템
	HelmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HelmMesh"));
	LegsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegsMesh"));
	ShoulderArmourMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShoulderArmourMesh"));
	FeetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FeetMesh"));
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandsMesh"));
	NecklaceMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NecklaceMesh"));
	BeltMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BeltMesh"));
	CloakMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CloakMesh"));

	// 모듈형 의상 시스템의 위계 설정
	HelmMesh->SetupAttachment(GetMesh());
	LegsMesh->SetupAttachment(GetMesh());
	ShoulderArmourMesh->SetupAttachment(GetMesh());
	FeetMesh->SetupAttachment(GetMesh());
	HeadMesh->SetupAttachment(GetMesh());
	HandsMesh->SetupAttachment(GetMesh());
	NecklaceMesh->SetupAttachment(GetMesh());
	BeltMesh->SetupAttachment(GetMesh());
	CloakMesh->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void AIVCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AIVCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AIVCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AIVCharacterBase::SetDead()
{
}

void AIVCharacterBase::SetAlive()
{
}

