// Fill out your copyright notice in the Description page of Project Settings.


#include "IVCharacterStatComponent.h"
#include "IVAN/Interface/IIVCharacterComponentProvider.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UIVCharacterStatComponent::UIVCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	MovementState = EMovementState::Idle;
	GaitState = EGaitState::Walk;
	JumpState = EJumpState::OnGround;
	TargetingState = ETargetingState::NonTargeting;
}

void UIVCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ĳ���� ������ ���� ȹ��� ������Ʈ
	AActor* Owner = GetOwner();
	if (Owner != nullptr && Owner->Implements<UIIVCharacterComponentProvider>())
	{
		IIIVCharacterComponentProvider* Provider = Cast<IIIVCharacterComponentProvider>(Owner);
		CharacterMovementComponent = Provider->GetCharacterMovementComponent();
	}
}

void UIVCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CharacterMovementComponent)
	{
		JumpState = (CharacterMovementComponent->IsFalling()) ? EJumpState::InAir : EJumpState::OnGround; // ���� ���� ����	
		MovementState = (CharacterMovementComponent->Velocity.Size2D() > 0.0f) ? EMovementState::Move : EMovementState::Idle; // ������ ���� ����
		// �������� ���� -> �÷��̾� ĳ���Ϳ��� ����
		// Ÿ���� ���� -> �÷��̾� ĳ���Ϳ��� ����
	}
}

