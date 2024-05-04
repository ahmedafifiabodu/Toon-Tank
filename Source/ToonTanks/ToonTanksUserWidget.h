// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/TextBlock.h>
#include "CoreMinimal.h"
#include <Components/CanvasPanel.h>
#include <Components/Image.h>
#include "Components/ProgressBar.h"
#include <Components/Button.h>
#include "Blueprint/UserWidget.h"
#include "Tank.h"
#include "MainGameModeBase.h"
#include "ToonTanksUserWidget.generated.h"

/**
 */
UCLASS()
class TOONTANKS_API UToonTanksUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UButton* StartButton;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UButton* ExitButton;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UCanvasPanel* SubCanvasPanel1;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UCanvasPanel* SubCanvasPanel2;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UCanvasPanel* SubCanvasPanel3;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UCanvasPanel* SubCanvasPanel4;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UTextBlock* ScoreText;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UTextBlock* CountdownText;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UImage* CountdownImage;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UProgressBar* HealthProgressBar;

private:

	UFUNCTION()
	void OnStartButtonClicked();
	UFUNCTION()
	void OnExitButtonClicked();

	int32 CountdownValue;
	float CountdownStartTime;
	bool bCountdownStarted;
	float CountdownTimeLeft;

	ATank* TankPawn = nullptr;
	APlayerController* PlayerController;
	AMainGameModeBase* GameMode;
};
