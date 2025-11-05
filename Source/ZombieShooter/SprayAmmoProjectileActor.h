// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieShooterProjectile.h"
#include "SprayAmmoProjectileActor.generated.h"

/*
 * This class inherits from the Zombie Shooter Projectile class.
 * This class represents the spray ammo projectile and is responsible for firing the correct spray ammo projectile and applying the correct damage when it collides with an enemy.
 */

UCLASS()
class ZOMBIESHOOTER_API ASprayAmmoProjectileActor : public AZombieShooterProjectile
{
	GENERATED_BODY()

public:
	ASprayAmmoProjectileActor();

	virtual void BeginPlay() override;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void OnDestroy();

	//Explosion Particle
	UPROPERTY(VisibleDefaultsOnly)
	class UParticleSystemComponent* Explosion;

	// Sound to play when the projectile destroys itself.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* ExplosionSound;

private:
	FTimerHandle SprayTimer;
};