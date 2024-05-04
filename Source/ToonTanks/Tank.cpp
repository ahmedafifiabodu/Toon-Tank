// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "InputCoreTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Components/CapsuleComponent.h"

#include "../../../../../../../../../../../../Program Files/Epic Games/UE_5.3/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../../../../../../../../../Program Files/Epic Games/UE_5.3/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

ATank* ATank::Instance = nullptr;

ATank* ATank::GetInstance()
{
	return Instance;
}

ATank::ATank()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	SpringArmComp->bInheritPitch = false; // Add this line
	SpringArmComp->bInheritYaw = false; // Add this line
	SpringArmComp->bInheritRoll = false; // Add this line

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComp);
	CameraComponent->bUsePawnControlRotation = false;

	Instance = this;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	PlayerConrollerRef = Cast<APlayerController>(GetController());

	Pc = GetWorld()->GetFirstPlayerController();
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerConrollerRef)
	{
		FHitResult TraceHitResult;
		bool bHitSomething = PlayerConrollerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, TraceHitResult);

		if (bHitSomething)
		{
			DrawDebugSphere(GetWorld(), TraceHitResult.ImpactPoint, 10, 15, FColor::Red);

			RotateTurret(TraceHitResult.ImpactPoint);
		}
	}

	if (bIsDestroyed)
	{
		this->SetActorHiddenInGame(true);
		this->SetActorTickEnabled(false);
	}

	CapsuleComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* MovementInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	MovementInput->BindAction(PlayerMovement, ETriggerEvent::Triggered, this, &ATank::Move);

	UEnhancedInputComponent* RunPressedStartedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	RunPressedStartedInput->BindAction(PlayerRun, ETriggerEvent::Triggered, this, &ATank::RunStarted);

	UEnhancedInputComponent* RunPressedCanceledInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	RunPressedCanceledInput->BindAction(PlayerRun, ETriggerEvent::Canceled, this, &ATank::RunCanceled);

	UEnhancedInputComponent* RunPressedCompleteInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	RunPressedCompleteInput->BindAction(PlayerRun, ETriggerEvent::Completed, this, &ATank::RunCanceled);

	UEnhancedInputComponent* JumpInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	JumpInput->BindAction(PlayerJump, ETriggerEvent::Started, this, &ATank::Jump);

	UEnhancedInputComponent* TurnInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	TurnInput->BindAction(PlayerTurn, ETriggerEvent::Triggered, this, &ATank::Turn);

	UEnhancedInputComponent* LookStartInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	LookStartInput->BindAction(PlayerLook, ETriggerEvent::Started, this, &ATank::LookStart);

	UEnhancedInputComponent* LookInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	LookInput->BindAction(PlayerLook, ETriggerEvent::Triggered, this, &ATank::Look);

	UEnhancedInputComponent* ShootInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	ShootInput->BindAction(PlayerShoot, ETriggerEvent::Started, this, &ATank::Fire);
}

void ATank::Move(const FInputActionValue& Value)
{
	// You can get the action value like this:

	bool BoolValue = Value.Get<bool>(); // for digital input actions
	float FloatValue = Value.Get<float>(); // for Axis1D input actions
	FVector2D Axis2DValue = Value.Get<FVector2D>(); // for Axis2D input actions
	FVector VectorValue = Value.Get<FVector>(); // for Axis3D input actions

	float CurrentSpeed = bIsRunning ? Speed * RunSpeedMultiplier : Speed;
	FVector MovementVelocity = GetActorForwardVector() * FloatValue * CurrentSpeed;
	CapsuleComp->SetPhysicsLinearVelocity(MovementVelocity);
}

void ATank::RunStarted(const FInputActionValue& Value)
{
	bIsRunning = true;
}

void ATank::RunCanceled(const FInputActionValue& Value)
{
	bIsRunning = false;
}

void ATank::Jump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Jump!"));
		FVector JumpForce = FVector(0, 0, JumpHeight);
		CapsuleComp->AddImpulse(JumpForce, NAME_None, true);
	}
}

void ATank::Turn(const FInputActionValue& Value)
{
	float FloatValue = Value.Get<float>();

	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = TurnRate * FloatValue * GetWorld()->GetDeltaSeconds();

	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::LookStart(const FInputActionValue& Value)
{
	bIsLookingRight = !bIsLookingRight;
}

void ATank::Look(const FInputActionValue& Value)
{
	float FloatValue = Value.Get<float>();

	FRotator NewRotation = SpringArmComp->GetComponentRotation();

	if (bIsLookingRight)
		NewRotation.Yaw += FloatValue;

	else
		NewRotation.Yaw -= FloatValue;

	SpringArmComp->SetWorldRotation(NewRotation);
}

void ATank::Fire()
{
	Super::Fire();

	if (ProjectileClass && Pc && FireShake)
		Pc->ClientStartCameraShake(FireShake);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	if (Pc && DeathShake)
		Pc->ClientStartCameraShake(DeathShake);
}