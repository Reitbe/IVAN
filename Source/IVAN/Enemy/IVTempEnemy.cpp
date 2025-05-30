// Fill out your copyright notice in the Description page of Project Settings.


#include "IVTempEnemy.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "IVAN/UI/IVBaseStatBar.h"

AIVTempEnemy::AIVTempEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// 루트 컴포넌트 지정
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

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

	// 체력 스탯 초기화
	MaxHealth = 500.0f;
	CurrentHealth = 500.0f;
}

void AIVTempEnemy::BeginPlay()
{
	Super::BeginPlay();
	HealthBar = Cast<UIVBaseStatBar>(WidgetComponent->GetUserWidgetObject());
}

void AIVTempEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AIVTempEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HealthBar)
	{
		CurrentHealth -= Damage;
		HealthBar->UpdateStatBar(MaxHealth, CurrentHealth);
	}
	return 0.0f;
}

