// Fill out your copyright notice in the Description page of Project Settings.


#include "PhaseAmmoProjectileActor.h"
#include "ZombieAICharacter.h"
#include "WeaponActor.h"
#include "ZombieShooterCharacter.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet\GameplayStatics.h"

// Sets default values
APhaseAmmoProjectileActor::APhaseAmmoProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Use a sphere as a simple collision representation
	CollisionComp->SetCollisionProfileName("OverlapAll");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APhaseAmmoProjectileActor::OnOverlapBegin);		// set up a notification for when this component overlaps something
}

void APhaseAmmoProjectileActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AZombieAICharacter* ZombieAIRef = Cast<AZombieAICharacter>(OtherActor);
	if (ZombieAIRef)
	{
		//Damage any enemy it passes through
		UGameplayStatics::ApplyDamage(ZombieAIRef, 10.0f, nullptr, nullptr, nullptr);
	}
}


