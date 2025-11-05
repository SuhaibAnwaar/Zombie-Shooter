// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieShooterProjectile.h"
#include "PhaseAmmoProjectileActor.generated.h"

/**
 * This class inherits from the Zombie Shooter Projectile class.
 * This class represents the phase ammo projectile and is responsible for firing the correct phase ammo projectile and applying the correct damage when it collides with an enemy.
 */

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class ZOMBIESHOOTER_API APhaseAmmoProjectileActor : public AZombieShooterProjectile
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	APhaseAmmoProjectileActor();

	// called when projectile overlaps something 
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
