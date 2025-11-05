// Fill out your copyright notice in the Description page of Project Settings.


#include "OriginalAmmoPickupActor.h"
#include "WeaponActor.h"
#include "ZombieShooterCharacter.h"
#include "HUDUserWidget.h"

#include "Components/TextRenderComponent.h"

AOriginalAmmoPickupActor::AOriginalAmmoPickupActor()
{
	//Call overlap function when the player overlaps the static mesh
	AmmoComp->OnComponentBeginOverlap.AddDynamic(this, &AOriginalAmmoPickupActor::OnOverlapBegin);
	AmmoLabel->SetText(TEXT("Original Ammo"));
}

void AOriginalAmmoPickupActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (PlayerRef == OtherActor)
	{
		//If player is currently using the orginal ammo then update the current ammo selected
		if (PlayerRef->Weapon->GetCurrentMultiAmmoType() == EMultiAmmoType::Original && PlayerRef->Weapon->GetCurrentAmmoType() == EAmmoType::MultiAmmo)
		{
			UpdateCurrentAmmo();
		}
		else//if the player has not got the orginal ammo selected then find ammo type in AllDefaultAmmo array and update the new ammo.
		{
			UpdateLastAmmo(0);
		}
	}
}