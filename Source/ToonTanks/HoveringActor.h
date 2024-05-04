// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HoveringActor.generated.h"

UCLASS()
class TOONTANKS_API AHoveringActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHoveringActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties");
	FVector Displacement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties");
	float Speed = 20.0f;

private:

	FVector OriginalLocation;
	FVector TargetLocation;
};
