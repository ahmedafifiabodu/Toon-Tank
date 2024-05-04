// Fill out your copyright notice in the Description page of Project Settings.

#include "ToonTanksUserWidget.h"
#include "GameFramework/PlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "HealthComponent.h"

void UToonTanksUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind the OnClicked event of the StartButton to the OnStartButtonClicked function
	if (StartButton)
		StartButton->OnClicked.AddDynamic(this, &UToonTanksUserWidget::OnStartButtonClicked);

	if (ExitButton)
		ExitButton->OnClicked.AddDynamic(this, &UToonTanksUserWidget::OnExitButtonClicked);

	PlayerController = GetWorld()->GetFirstPlayerController();
	GameMode = Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode());

	if (SubCanvasPanel3)
		SubCanvasPanel3->SetVisibility(ESlateVisibility::Hidden);

	if (SubCanvasPanel4)
		SubCanvasPanel4->SetVisibility(ESlateVisibility::Hidden);
}

void UToonTanksUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bCountdownStarted)
	{
		CountdownTimeLeft -= InDeltaTime;

		if (CountdownTimeLeft <= CountdownValue - 1)
		{
			--CountdownValue;

			if (CountdownText)
			{
				FText CountdownTextValue;
				switch (CountdownValue)
				{
				case 3:
					CountdownTextValue = FText::FromString(TEXT("Get Ready!"));
					break;
				case 2:
					CountdownTextValue = FText::FromString(TEXT("Steady"));
					break;
				case 1:
					CountdownTextValue = FText::FromString(TEXT("GO !!"));
					break;
				default:
					CountdownTextValue = FText::FromString(TEXT(""));
					break;
				}
				CountdownText->SetText(CountdownTextValue);
			}

			if (CountdownValue <= 0)
			{
				bCountdownStarted = false;

				if (SubCanvasPanel1)
					SubCanvasPanel1->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}

	if (TankPawn && TankPawn->IsPlayerDead())
	{
		bCountdownStarted = false;

		if (SubCanvasPanel3)
			SubCanvasPanel3->SetVisibility(ESlateVisibility::Hidden);

		if (SubCanvasPanel4)
		{
			SubCanvasPanel4->SetVisibility(ESlateVisibility::Hidden);
		}

		if (CountdownImage)
		{
			FLinearColor RedColor(0.098958f, 0.0f, 0.014583f, 0.75f);
			CountdownImage->SetColorAndOpacity(RedColor);
		}

		if (SubCanvasPanel1)
		{
			SubCanvasPanel1->SetVisibility(ESlateVisibility::Visible);
		}

		if (CountdownText)
		{
			FText CountdownTextValue = FText::FromString(TEXT("Game Over"));
			CountdownText->SetText(CountdownTextValue);

			// Delay before pausing the game
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
				{
					if (PlayerController)
						PlayerController->SetPause(true);
				}, 1.f, false);
		}
	}

	if (GameMode && ScoreText)
	{
		int32 DestroyedTurretCount = GameMode->GetDestoryTurretCount();
		int32 TotalTurretCount = GameMode->GetTotalTurretCount();

		FText TurretCountTextValue = FText::Format(FText::FromString(TEXT("Turrets Destroyed: {0}/{1}")), DestroyedTurretCount, TotalTurretCount);
		ScoreText->SetText(TurretCountTextValue);

		if (DestroyedTurretCount == TotalTurretCount)
		{
			if (SubCanvasPanel1)
			{
				SubCanvasPanel1->SetVisibility(ESlateVisibility::Visible);
			}

			if (SubCanvasPanel3)
				SubCanvasPanel3->SetVisibility(ESlateVisibility::Hidden);

			if (SubCanvasPanel4)
				SubCanvasPanel4->SetVisibility(ESlateVisibility::Hidden);

			if (CountdownImage)
			{
				FLinearColor GreenColor(0.0f, 0.171875f, 0.001086f, 0.75f);
				CountdownImage->SetColorAndOpacity(GreenColor);
			}

			if (CountdownText)
			{
				FText CountdownTextValue = FText::FromString(TEXT("Victory"));
				CountdownText->SetText(CountdownTextValue);

				if (PlayerController)
					PlayerController->SetPause(true);
			}
		}
	}

	if (TankPawn)
	{
		UHealthComponent* HealthComponent = TankPawn->FindComponentByClass<UHealthComponent>();
		if (HealthComponent && HealthProgressBar)
		{
			HealthProgressBar->SetPercent(HealthComponent->GetHealthPercent());
		}
	}
}

void UToonTanksUserWidget::OnStartButtonClicked()
{
	if (SubCanvasPanel1)
		SubCanvasPanel1->SetVisibility(ESlateVisibility::Visible);

	if (SubCanvasPanel2)
		SubCanvasPanel2->RemoveFromParent();

	if (SubCanvasPanel3)
		SubCanvasPanel3->SetVisibility(ESlateVisibility::Visible);

	if (SubCanvasPanel4)
		SubCanvasPanel4->SetVisibility(ESlateVisibility::Visible);

	if (CountdownImage)
	{
		FLinearColor RedColor(0.0f, 0.0f, 0.0f, 0.0f);
		CountdownImage->SetColorAndOpacity(RedColor);
	}

	CountdownValue = 3;
	CountdownTimeLeft = 3.0f;
	bCountdownStarted = true;

	if (CountdownText)
	{
		FText CountdownTextValue = FText::FromString(TEXT("Get Ready!"));
		CountdownText->SetText(CountdownTextValue);
	}

	if (PlayerController)
		PlayerController->SetPause(false);

	TankPawn = ATank::GetInstance();
}

void UToonTanksUserWidget::OnExitButtonClicked()
{
	if (APlayerController* Pc = GetOwningPlayer())
		Pc->ConsoleCommand("quit");
}