// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieShooterProjectile.h"
#include "MultiAmmoProjectileActor.generated.h"

/**
 * This class inherits from the Zombie Shooter Projectile class.
 * This class represents the multi ammo projectile and is responsible for firing the correct multi ammo projectile and applying the correct damage when it collides with an enemy.
 */

UCLASS()
class ZOMBIESHOOTER_API AMultiAmmoProjectileActor : public AZombieShooterProjectile
{
	GENERATED_BODY()
	
public:
	AMultiAmmoProjectileActor();

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Particles")
		TSubclassOf<class AIceParticleActor> IceParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Particles")
		TSubclassOf<class AFireParticleActor> FireParticle;
};
