// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainGameModeBase.generated.h"

/**
 *
 *
 */
UCLASS()
class TOONTANKS_API AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	void ActorDied(AActor* DeadActor);

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game UI")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	void IncrementDestoryTurretCount() { DestoryTurretCount++; }
	int32 GetDestoryTurretCount() const { return DestoryTurretCount; }

	void IncrementTotalTurretCount() { TotalTurretCount++; }
	int32 GetTotalTurretCount() const { return TotalTurretCount; }

private:
	int32 TotalTurretCount = 0;
	int32 DestoryTurretCount = 0;

	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase* BackgroundMusic;
};
