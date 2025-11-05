// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoBoxActor.generated.h"

/*
This class is responsible for have the base functionality to refill any ammo type of the players. 
The children of this class will specfiy which ammo needs to be filled and then call the appropriate functions in this class.
This class also represents how each ammo pickup will look like.
*/

class UStaticMeshComponent;
class UTextRenderComponent;

UCLASS()
class ZOMBIESHOOTER_API AAmmoBoxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoBoxActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Static mesh that represents the Ammo
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* AmmoComp;

	//The text that displays what the ammo type is
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTextRenderComponent* AmmoLabel;

	//Player Reference
	class AZombieShooterCharacter* PlayerRef;

	//functions
	void UpdateCurrentAmmo();

	//if the player has not got the ammo selected then update the ammo in the inventory.
	void UpdateLastAmmo(int AmmoIndex);
};
