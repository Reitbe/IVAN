// Fill out your copyright notice in the Description page of Project Settings.


#include "IVCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

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

void AIVCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AIVCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIVCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AIVCharacterBase::SetDead()
{
	// 캡슐 콜리전 처리
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 물리 시뮬레이션 활성화
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetAllBodiesPhysicsBlendWeight(1.0f);
	GetMesh()->bBlendPhysics = true;

	// 메쉬 콜리전 래그돌 전환
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	// 무브먼트 중지
	GetCharacterMovement()->SetMovementMode(MOVE_None);
}

void AIVCharacterBase::SetAlive()
{
}

