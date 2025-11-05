// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiAmmoProjectileActor.h"
#include "ZombieShooterCharacter.h"
#include "IceParticleActor.h"
#include "FireParticleActor.h"
#include "WeaponActor.h"
#include "ZombieAICharacter.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet\GameplayStatics.h"

AMultiAmmoProjectileActor::AMultiAmmoProjectileActor()
{
	CollisionComp->OnComponentHit.AddDynamic(this, &AMultiAmmoProjectileActor::OnHit);		// set up a notification for when this component hits something blocking
}

void AMultiAmmoProjectileActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Get Reference to play to access its current ammo type
	AZombieShooterCharacter* PlayerRef = Cast<AZombieShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	AZombieAICharacter* ZombieRef = Cast<AZombieAICharacter>(OtherActor);

	if (ZombieRef)
	{
		if (PlayerRef && PlayerRef->Weapon)
		{
			//Particles will overlap anything so always spawn
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			//The original ammo type will apply initial damage whereas the other two will apply continuous damage for a few seconds.
			switch (PlayerRef->Weapon->GetCurrentMultiAmmoType())
			{
			case EMultiAmmoType::Original:
				UGameplayStatics::ApplyDamage(ZombieRef, 20.0f, nullptr, nullptr, nullptr);
				break;
			case EMultiAmmoType::Ice:
				if (IceParticle)
				{
					GetWorld()->SpawnActor<AIceParticleActor>(IceParticle, GetActorLocation(), GetActorRotation(), SpawnParams);
				}
				break;
			case EMultiAmmoType::Fire:
				if (FireParticle)
				{
					GetWorld()->SpawnActor<AFireParticleActor>(FireParticle, GetActorLocation(), GetActorRotation(), SpawnParams);
				}
				break;
			default:
				break;
			}
		}
		Destroy();
	}
	else
	{
		Destroy();
	}

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}