// Fill out your copyright notice in the Description page of Project Settings.


#include "HoveringActor.h"

// Sets default values
AHoveringActor::AHoveringActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHoveringActor::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalLocation = GetActorLocation();
	TargetLocation = OriginalLocation + Displacement;
}

// Called every frame
void AHoveringActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, DeltaTime, Speed);
	SetActorLocation(NewLocation);

	if (GetActorLocation().Equals(TargetLocation, 1.0f))
	{
		FVector Swap = OriginalLocation;
		OriginalLocation = TargetLocation;
		TargetLocation = Swap;
	}
}


