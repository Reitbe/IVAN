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

	// 위젯 출력용
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);

	// 출력할 위젯 클래스 지정
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetFinder
	(TEXT("/Game/Widget/WBP_HpBar.WBP_HpBar_C"));
	if (WidgetFinder.Class)
	{
		WidgetComponent->SetWidgetClass(WidgetFinder.Class);
		WidgetComponent->SetDrawSize(FVector2D(200.0f, 30.0f));
		WidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	}

	// 임시 스탯
	MaxHealth = 100.0f;
	CurrentHealth = 100.0f;
}

// Called when the game starts or when spawned
void AIVTempEnemy::BeginPlay()
{
	Super::BeginPlay();
	HealthBar = Cast<UIVBaseStatBar>(WidgetComponent->GetUserWidgetObject());
}

// Called every frame
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

