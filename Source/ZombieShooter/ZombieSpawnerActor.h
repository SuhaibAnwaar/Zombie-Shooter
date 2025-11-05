// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawnerActor.generated.h"

UCLASS()
class ZOMBIESHOOTER_API AZombieSpawnerActor : public AActor
{
	GENERATED_BODY()
	
private:	
	//Current number of zombies spawned
	int ZombieCount;

	//only start spawning zombies if this variable is true.
	bool bCanSpawnZombies;

	//How often a zombie spawns.
	float ZombieSpawnInterval;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AZombieSpawnerActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Reference to the zombie class
	UPROPERTY(EditDefaultsOnly)
		 TSubclassOf<class AZombieAICharacter> ZombieAI;

	//Max number of zombies that can spawn
	UPROPERTY(EditDefaultsOnly)
		int MaxZombieCount;
	
	void SpawnZombie();

	//Setters 
	void SetZombieCount(int amount);
	void SetCanSpawnZombies(bool statement);
	void SetZombieSpawnInterval(float amount);

	//Getters
	int GetZombieCount();
	bool GetCanSpawnZombies();
	float GetZombieSpawnInterval();
};
