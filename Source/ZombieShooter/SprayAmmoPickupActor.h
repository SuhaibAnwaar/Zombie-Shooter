// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoBoxActor.h"
#include "SprayAmmoPickupActor.generated.h"

/*
This class inherits from the Ammo box class.
This class represents the spray ammo and is responsible for refilling the players spray ammo whenever the overlap with this mesh.
*/

UCLASS()
class ZOMBIESHOOTER_API ASprayAmmoPickupActor : public AAmmoBoxActor
{
	GENERATED_BODY()
	
public:
	ASprayAmmoPickupActor();

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
