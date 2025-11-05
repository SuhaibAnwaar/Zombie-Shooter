// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoBoxActor.h"
#include "IceAmmoPickupActor.generated.h"

/**
 * 
 This class inherits from the Ammo box class. 
 This class represents the ice ammo and is responsible for refilling the player's ice ammo whenever they overlap with this mesh.
 */
UCLASS()
class ZOMBIESHOOTER_API AIceAmmoPickupActor : public AAmmoBoxActor
{
	GENERATED_BODY()

public:
	AIceAmmoPickupActor();

	//functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
