// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 *
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	bool IsPlayerDead() const { return bIsPlayerDead; }
	void SetPlayerDead(bool bDead) { bIsPlayerDead = bDead; }

	static ATank* GetInstance();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputAction* PlayerMovement = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputAction* PlayerRun = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputAction* PlayerJump = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputAction* PlayerTurn = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputAction* PlayerLook = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputAction* PlayerShoot = nullptr;

private:

	bool bIsLookingRight = true;
	bool bIsPlayerDead = false;
	static ATank* Instance;
	class APlayerController* PlayerConrollerRef;
	class APlayerController* Pc;

	UPROPERTY(EditAnywhere);
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere);
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 300;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 50;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunSpeedMultiplier = 2.0f;
	bool bIsRunning = false;

	UPROPERTY(EditAnywhere, Category = "Movement")
	double JumpHeight = 20;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UCameraShakeBase> FireShake;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UCameraShakeBase> DeathShake;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void RunStarted(const FInputActionValue& Value);

	UFUNCTION()
	void RunCanceled(const FInputActionValue& Value);

	UFUNCTION()
	void Jump(const FInputActionValue& Value);

	UFUNCTION()
	void Turn(const FInputActionValue& Value);

	UFUNCTION()
	void LookStart(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Fire() override;

	void HandleDestruction() override;
};
