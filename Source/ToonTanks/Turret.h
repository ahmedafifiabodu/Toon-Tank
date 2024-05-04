// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Turret.generated.h"

/**
 *
 */
class ATank;

UCLASS()
class TOONTANKS_API ATurret : public ABasePawn
{
	GENERATED_BODY()

private:

	void BeginPlay();
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction() override;

	ATank* Tank;

	FRotator OriginalRotation;
	float Range = 1000.0f;

	FTimerHandle FireTimerHandle;
};
