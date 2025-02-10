// Fill out your copyright notice in the Description page of Project Settings.


#include "IVEnemy.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "IVAN/Stat/IVEquipComponent.h"
#include "IVAN/Stat/IVMonsterStatComponent.h"
#include "IVAN/Attack/IVHitReactionComponent.h"
#include "IVAN/Attack/IVAttackComponent.h"
#include "IVAN/GameSystem/IVDeathEventSubsystem.h"
#include "IVAN/Item/IVWeapon.h"
#include "IVAN/UI/IVBaseStatBar.h"

AIVEnemy::AIVEnemy()
{
	// 스탯 컴포넌트 설정
	MonsterStatComponent = CreateDefaultSubobject<UIVMonsterStatComponent>(TEXT("MonsterStatComponent"));

	// 장비 관리용 컴포넌트
	EquipComponent = CreateDefaultSubobject<UIVEquipComponent>(TEXT("EquipComponent"));

	// 공격 및 피격 관련 컴포넌트
	AttackComponent = CreateDefaultSubobject<UIVAttackComponent>(TEXT("AttackComponent"));
	HitReactionComponent = CreateDefaultSubobject<UIVHitReactionComponent>(TEXT("HitReactionComponent"));

	// 체력 바 출력용 위젯 컴포넌트
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);

	// 출력할 위젯 클래스 지정
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetFinder
	(TEXT("/Game/Widget/WBP_HpBar.WBP_HpBar_C"));
	if (WidgetFinder.Class)
	{
		WidgetComponent->SetWidgetClass(WidgetFinder.Class);
		WidgetComponent->SetDrawSize(FVector2D(100.0f, 15.0f));
		WidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	}
}

void AIVEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIVEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AIVEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 애님 인스턴스 설정
	AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		HitReactionComponent->SetAnimInstance(AnimInstance);
		AttackComponent->SetAnimInstance(AnimInstance);
	}

	// 위젯 바인딩
	if (MonsterStatComponent && WidgetComponent)
	{
		HealthBar = Cast<UIVBaseStatBar>(WidgetComponent->GetUserWidgetObject());
		if (HealthBar)
		{
			MonsterStatComponent->OnHpChanged.BindUObject(HealthBar, &UIVBaseStatBar::UpdateStatBar);
		}
	}

	// 몬스터 사망 이벤트 바인딩
	if (MonsterStatComponent)
	{
		MonsterStatComponent->OnMonsterDeathLocalEvent.AddUObject(this, &AIVEnemy::SetDead);
	}
}

void AIVEnemy::SetDead()
{
	Super::SetDead();

	// 사망 애니메이션 재생 -> 래그돌로 대체
	//if (HitReactionComponent)
	//{
	//	HitReactionComponent->PlayDeathMontage();
	//}

	// 래그돌 처리
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetSimulatePhysics(true);

	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetAllBodiesPhysicsBlendWeight(1.0f);
	GetMesh()->bBlendPhysics = true;

	// UI 처리
	if (HealthBar)
	{
		HealthBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AIVEnemy::SetAlive()
{
	Super::SetAlive();
}

float AIVEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	

	// 피격 이벤트 처리
	if (MonsterStatComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser))
	{
		// 피격 과정에서 공격 종료가 먼저 호출되도록 if문에서 진행
		if (HitReactionComponent->ComputeHitAngle(Damage, DamageEvent, EventInstigator, DamageCauser))
		{
			// AI컨트롤러에 피격 이벤트 전달
			IIIVAIControllerBasicCombat* AIController = Cast<IIIVAIControllerBasicCombat>(GetController());
			if (AIController)
			{
				AActor* Attacker = EventInstigator ? EventInstigator->GetPawn() : nullptr;
				if (Attacker)
				{
					AIController->OnHit(Attacker);
				}
			}
		}
	}
	return 0.0f;
}

TObjectPtr<AIVWeapon> AIVEnemy::GetWeapon() const
{
	// 장비 컴포넌트로부터 무기를 받아 전달한다
	if (EquipComponent)
	{
		return EquipComponent->GetWeapon();
	}
	return nullptr;
}

void AIVEnemy::AttackEnd(bool bIsFirstCheck)
{
	// Notify에 의한 공격 종료는 플레이어의 콤보를 위한 것
	if (bIsFirstCheck) return;

	// 몽타주가 중간에 끊기거나(피격) 온전히 끝난 경우에만 AI 컨트롤러에 공격 종료 전달
	IIIVAIControllerBasicCombat* AIController = Cast<IIIVAIControllerBasicCombat>(GetController());
	if (AIController)
	{
		AIController->OnAttackEnd();
	}
}

void AIVEnemy::EquipByClass(TSubclassOf<AIVItemBase> Item) const
{

}

void AIVEnemy::EquipByInstance(TObjectPtr<AIVItemBase> Item) const
{
	// 전달받은 아이템을 지정 위치에 부착한다
	if (Item)
	{
		/*
		* 아이템은 본인이 장착될 위치를 가지고 있다.
		* 현재는 임시로 플레이어 캐릭터의 특정 소켓을 사용한다.
		*/
		FName SocketName(TEXT("hand_rSocket"));
		if (!GetMesh()->DoesSocketExist(SocketName))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("캐릭터에 해당 소켓이 없습니다."));
			return;
		}
		Item->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	}

	// 아이템이 무기라면 공격 컴포넌트에 무기를 설정한다
	if (Item->GetItemType() == EItemType::Weapon && AttackComponent)
	{
		AttackComponent->SetWeapon(Cast<AIVWeapon>(Item));
	}
}

void AIVEnemy::StartHitReaction()
{
}

void AIVEnemy::EndHitReaction()
{
	// AI 컨트롤러에 공격 종료 전달
	IIIVAIControllerBasicCombat* AIController = Cast<IIIVAIControllerBasicCombat>(GetController());
	if (AIController)
	{
		AIController->OnHitEnd();
	}
}

void AIVEnemy::EndDeathReaction()
{
	// 래그돌
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetSimulatePhysics(true);
}

void AIVEnemy::ExecuteBasicAttack()
{

	// 스탯 컴포넌트로부터 몬스터의 데미지 정보를 가져온다
	FBaseDamageStat BaseDamageStat;
	if (MonsterStatComponent)
	{
		BaseDamageStat = MonsterStatComponent->GetBaseDamageStat();
	}

	if (AttackComponent)
	{
		AttackComponent->Attack(BaseDamageStat);
	}
}
