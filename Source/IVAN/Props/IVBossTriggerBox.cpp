// Fill out your copyright notice in the Description page of Project Settings.


#include "IVBossTriggerBox.h"
#include "Components/ShapeComponent.h"
#include "IVAN/Character/IVPlayerCharacter.h"
#include "IVAN/Enemy/IVBossEnemy.h"
#include "IVAN/Controller/IVPlayerController.h"

AIVBossTriggerBox::AIVBossTriggerBox()
{
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AIVBossTriggerBox::OnOverlapBegin);
	GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &AIVBossTriggerBox::OnOverlapEnd);
}

void AIVBossTriggerBox::BeginPlay()
{
	Super::BeginPlay();
}

void AIVBossTriggerBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Overlap Begin"));

	if (OtherActor->ActorHasTag("Player"))
	{
		PlayerCharacter = Cast<AIVPlayerCharacter>(OtherActor);
	}

	if (PlayerCharacter) 
	{
		AIVPlayerController* PlayerController = Cast<AIVPlayerController>(PlayerCharacter->GetController());
		if (PlayerController && BossEnemy.IsValid())
		{
			PlayerController->ShowBossStatWidget(BossEnemy.Get());
		}
	}
	PlayerCharacter = nullptr;
}

void AIVBossTriggerBox::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Overlap End"));

	if (OtherActor->ActorHasTag("Player"))
	{
		PlayerCharacter = Cast<AIVPlayerCharacter>(OtherActor);
	}

	if (PlayerCharacter)
	{
		AIVPlayerController* PlayerController = Cast<AIVPlayerController>(PlayerCharacter->GetController());
		if (PlayerController && BossEnemy.IsValid())
		{
			PlayerController->HideBossStatWidget();
		}
	}
	PlayerCharacter = nullptr;
}
