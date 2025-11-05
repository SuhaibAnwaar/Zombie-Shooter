// Fill out your copyright notice in the Description page of Project Settings.


#include "FireAmmoPickupActor.h"
#include "WeaponActor.h"
#include "ZombieShooterCharacter.h"
#include "HUDUserWidget.h"

#include "Components/TextRenderComponent.h"

AFireAmmoPickupActor::AFireAmmoPickupActor()
{
	//Call overlap function when the player overlaps the static mesh
	AmmoComp->OnComponentBeginOverlap.AddDynamic(this, &AFireAmmoPickupActor::OnOverlapBegin);
	AmmoLabel->SetText(TEXT("Fire Ammo"));
}

void AFireAmmoPickupActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (PlayerRef == OtherActor)
	{
		//If player is currently using the fire ammo then update the current ammo selected
		if (PlayerRef->Weapon->GetCurrentMultiAmmoType() == EMultiAmmoType::Fire && PlayerRef->Weapon->GetCurrentAmmoType() == EAmmoType::MultiAmmo)
		{
			UpdateCurrentAmmo();
		}
		else//if the player has not got the fire ammo selected then find ammo type in AllDefaultAmmo array and update the new ammo.
		{
			UpdateLastAmmo(2);
		}
	}
}