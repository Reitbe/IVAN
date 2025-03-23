// Fill out your copyright notice in the Description page of Project Settings.


#include "IVInteractionComponent.h"

// Sets default values for this component's properties
UIVInteractionComponent::UIVInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	InteractionDistance = 150.0f;
}

void UIVInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UIVInteractionComponent::SearchAndInteract()
{
	// 상호작용 대상 탐색을 위한 콜리전 설정
	TArray<FHitResult> HitResults;
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start; // 원점
	FQuat Rotation = FQuat::Identity;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(InteractionDistance);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	if (GetWorld()->SweepMultiByObjectType(
		HitResults,
		Start,
		End,
		Rotation,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic), 
		CollisionShape,
		QueryParams))
	{
		AActor* ClosestActor = nullptr;
		float ClosestDistance = InteractionDistance;

		// 상호작용 대상 중 가장 가까운 액터 선택
		for (const FHitResult& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor && HitActor->Implements<UIIVInteractableInterface>()) // 상호작용 대상 조건 확인
			{
				if (IsTargetInteractable(HitActor)) // 시야에 있는지 확인
				{
					float Distance = FVector::Distance(Start, HitActor->GetActorLocation());
					if (Distance < ClosestDistance)
					{
						ClosestDistance = Distance;
						ClosestActor = HitActor;
					}
				}
			}
		}

		// 상호작용 실시
		if (ClosestActor)
		{
			IIIVInteractableInterface* Interactable = Cast<IIIVInteractableInterface>(ClosestActor);
			if (Interactable)
			{
				Interactable->Interact(GetOwner());
			}
		}
	}
}

bool UIVInteractionComponent::IsTargetInteractable(AActor* Target)
{
	if (!Target) return false;

	FVector TargetLocation = Target->GetActorLocation();
	FHitResult SightResult;
	FCollisionQueryParams SightParams;
	SightParams.AddIgnoredActor(GetOwner());
	SightParams.AddIgnoredActor(Target);

	// 타깃과 본인 사이가 가로막혀있다면 시야에 없는 것으로 판단
	bool bCanSee = !GetWorld()->LineTraceSingleByChannel(
		SightResult,
		GetOwner()->GetActorLocation(),
		TargetLocation,
		ECC_Visibility,
		SightParams
	);

	return bCanSee;
}

void UIVInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

