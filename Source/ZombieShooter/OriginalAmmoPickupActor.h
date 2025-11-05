// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoBoxActor.h"
#include "OriginalAmmoPickupActor.generated.h"

/**
 * 
 This class inherits from the Ammo box class. 
 This class represents the original ammo and is responsible for refilling the players original ammo whenever the overlap with this mesh.
 */
UCLASS()
class ZOMBIESHOOTER_API AOriginalAmmoPickupActor : public AAmmoBoxActor
{
	GENERATED_BODY()
	
public:
	AOriginalAmmoPickupActor();

	//functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
