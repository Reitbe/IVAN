// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "IVAN/GameSystem/IVSaveGame.h"
#include "IVSaveManagerSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSaveRequested, UIVSaveGame* /*SaveGame*/)

/**
 * 게임의 세이브 & 로드를 담당하는 서브시스템.
 */
UCLASS()
class IVAN_API UIVSaveManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

// 기본
public:
	UIVSaveManagerSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;


// 세이브
public:
	/* 세이브를 위해 저장 파일 주소를 각 요소들에게 전달하는 대리자 */
	FOnSaveRequested OnSaveRequested;

	/* 대리자를 구독중인 각 요소들에게 저장을 요청한다 */
	UFUNCTION(BlueprintCallable, Category = "Save")
	void RequestSaveGame();

	/* 플레이어 위치 저장 */
	void SaveTransform(const FTransform& Transform);

private:
	/* 세이브 게임 객체 */
	UPROPERTY()
	TObjectPtr<UIVSaveGame> SaveGameFile;


// 로드
public:
	UFUNCTION(BlueprintCallable, Category = "Load")
	EStageState GetStageState() const;

	UFUNCTION(BlueprintCallable, Category = "Load")
	FTransform GetTransform() const;

	UFUNCTION(BlueprintCallable, Category = "Load")
	TArray<FItemBaseInfo> GetInventorySlots() const;

	UFUNCTION(BlueprintCallable, Category = "Load")
	TArray<FItemBaseInfo> GetQuickSlots() const;

	UFUNCTION(BlueprintCallable, Category = "Load")
	TArray<FItemBaseInfo> GetEquipSlots() const;

	UFUNCTION(BlueprintCallable, Category = "Load")
	TArray<FItemBaseInfo> GetWeaponSlots() const;
};
