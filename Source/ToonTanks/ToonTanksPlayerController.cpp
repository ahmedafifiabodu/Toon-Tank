// Fill out your copyright notice in the Description page of Project Settings.

#include "ToonTanksPlayerController.h"

void AToonTanksPlayerController::HandleGameStart()
{
	if (GetPawn() != nullptr)
		GetPawn()->DisableInput(this);

	bShowMouseCursor = false;
	bEnableClickEvents = false;
 
	FTimerDelegate TimerDel;

	TimerDel.BindUFunction(this, FName("SetPlayerEnabledState"));

	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, TimerDel, 3.0f, false);
}

void AToonTanksPlayerController::SetPlayerEnabledState()
{

	if (GetPawn() != nullptr)
		GetPawn()->EnableInput(this);

	bShowMouseCursor = true;
	bEnableClickEvents = true;
}