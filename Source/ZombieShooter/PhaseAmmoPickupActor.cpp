// Fill out your copyright notice in the Description page of Project Settings.


#include "PhaseAmmoPickupActor.h"
#include "WeaponActor.h"
#include "ZombieShooterCharacter.h"
#include "HUDUserWidget.h"

#include "Components/TextRenderComponent.h"

APhaseAmmoPickupActor::APhaseAmmoPickupActor()
{
	//Call overlap function when the player overlaps the static mesh
	AmmoComp->OnComponentBeginOverlap.AddDynamic(this, &APhaseAmmoPickupActor::OnOverlapBegin);
	AmmoLabel->SetText(TEXT("Phase Ammo"));
}

void APhaseAmmoPickupActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (PlayerRef == OtherActor)
	{
		//If player is currently using the spray ammo then update the current ammo selected
		if (PlayerRef->Weapon->GetCurrentAmmoType() == EAmmoType::PhaseAmmo)
		{
			UpdateCurrentAmmo();
		}
		else//if the player has not got the spray ammo selected then find ammo type in AllDefaultAmmo array and update the new ammo.
		{
			UpdateLastAmmo(3);
		}
	}
}
