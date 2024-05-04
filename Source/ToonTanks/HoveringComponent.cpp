// Fill out your copyright notice in the Description page of Project Settings.

#include "HoveringComponent.h"

// Sets default values for this component's properties
UHoveringComponent::UHoveringComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UHoveringComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (GetOwner())
	{
		OriginalLocation = GetOwner()->GetActorLocation();
		TargetLocation = OriginalLocation + Displacement;
	}
}

// Called every frame
void UHoveringComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (GetOwner())
	{
		FVector NewLocation = FMath::VInterpConstantTo(GetOwner()->GetActorLocation(), TargetLocation, DeltaTime, Speed);
		GetOwner()->SetActorLocation(NewLocation);

		if (GetOwner()->GetActorLocation().Equals(TargetLocation, 1.0f))
		{
			FVector Swap = OriginalLocation;
			OriginalLocation = TargetLocation;
			TargetLocation = Swap;
		}
	}
}