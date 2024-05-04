// Fill out your copyright notice in the Description page of Project Settings.

#include "MainGameModeBase.h"
#include "BasePawn.h"
#include "Tank.h"
#include "Turret.h"
#include <Kismet/GameplayStatics.h>

void AMainGameModeBase::ActorDied(AActor* DeadActor)
{
	ABasePawn* BasePawn = Cast<ABasePawn>(DeadActor);
	if (BasePawn)
	{
		BasePawn->HandleDestruction();

		ATank* TankPawn = Cast<ATank>(BasePawn);
		if (TankPawn)
			TankPawn->SetPlayerDead(true);
	}
	else
	{
		DeadActor->Destroy();
	}
}

void AMainGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (StartingWidgetClass)
	{
		UUserWidget* StartingWidget = CreateWidget<UUserWidget>(GetWorld(), StartingWidgetClass);
		if (StartingWidget)
		{
			StartingWidget->AddToViewport();

			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController)
			{
				PlayerController->SetPause(true);
				PlayerController->bShowMouseCursor = true;
			}
		}
	}

	if (BackgroundMusic)
	{
		UGameplayStatics::PlaySound2D(this, BackgroundMusic);
	}

	TArray<AActor*> TurretActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATurret::StaticClass(), TurretActors);
	TotalTurretCount = TurretActors.Num();
}