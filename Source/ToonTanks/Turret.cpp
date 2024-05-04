// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"
#include "Tank.h"
#include "MainGameModeBase.h"

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	// Get a reference to the Tank
	Tank = ATank::GetInstance();

	OriginalRotation = GetActorRotation();
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Tank && !bIsDestroyed)
	{
		float Distance = FVector::Dist(Tank->GetActorLocation(), GetActorLocation());

		FHitResult HitResult;
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), Tank->GetActorLocation(), ECC_Visibility, TraceParams);

		if (Distance <= Range && bHit && HitResult.GetActor() == Tank)
		{
			FVector Direction = Tank->GetActorLocation() - GetActorLocation();
			Direction.Normalize();

			FRotator NewRotation = Direction.Rotation();
			NewRotation.Pitch = 0;
			NewRotation.Roll = 0;

			FRotator SmoothRotation = FMath::RInterpTo(TurretMesh->GetComponentRotation(), NewRotation, DeltaTime, 2.0f);

			TurretMesh->SetWorldRotation(SmoothRotation);

			if (!GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
				GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ATurret::Fire, 2.0f, true);
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);

			float RotationAngle = FMath::Sin(GetWorld()->GetTimeSeconds()) * 90.0f;
			FRotator OscillatingRotation = OriginalRotation;
			OscillatingRotation.Yaw += RotationAngle;

			TurretMesh->SetWorldRotation(OscillatingRotation);
		}
	}

	if (bIsDestroyed)
	{
		this->SetActorEnableCollision(false);
		this->SetActorHiddenInGame(true);
		this->SetActorTickEnabled(false);
	}
}

void ATurret::HandleDestruction()
{
	Super::HandleDestruction();

	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);

	AMainGameModeBase* GameMode = Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode());

	if (GameMode)
		GameMode->IncrementDestoryTurretCount();
}