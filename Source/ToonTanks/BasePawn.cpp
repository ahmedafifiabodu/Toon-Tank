// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "ToonTanksPlayerController.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ABasePawn::ABasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectilePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Point"));
	ProjectilePoint->SetupAttachment(TurretMesh);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();

	AToonTanksPlayerController* PlayerController = Cast<AToonTanksPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController != nullptr)
	{
		PlayerController->HandleGameStart();
	}
}

void ABasePawn::Fire()
{
	if (ProjectileClass)
	{
		FVector Location = ProjectilePoint->GetComponentLocation();
		FRotator Rotation = ProjectilePoint->GetComponentRotation();

		AProjectile* TempProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);

		if (FireParticle)
			UGameplayStatics::SpawnEmitterAtLocation(this, FireParticle, TempProjectile->GetActorLocation(), TempProjectile->GetActorRotation());
	}
}

void ABasePawn::HandleDestruction()
{
	bIsDestroyed = true;

	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();

	FRotator LookAtRoataion = ToTarget.Rotation();
	LookAtRoataion.Roll = LookAtRoataion.Pitch = 0;

	TurretMesh->SetWorldRotation(LookAtRoataion);
}