// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Projectile.h"
#include "BasePawn.generated.h"

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	ABasePawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RotateTurret(FVector LookAtTarget);

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Type")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* FireParticle;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* DeathParticle;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* DeathSound;

	virtual void HandleDestruction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Fire();

	bool bIsDestroyed = false;

	UPROPERTY(EditAnywhere);
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere);
	class USceneComponent* ProjectilePoint;

	UPROPERTY(EditAnywhere);
	class UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere);
	class UStaticMeshComponent* BaseMesh;
};
