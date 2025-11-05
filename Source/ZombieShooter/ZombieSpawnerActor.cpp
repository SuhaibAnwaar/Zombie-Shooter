// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSpawnerActor.h"
#include "ZombieAICharacter.h"

// Sets default values
AZombieSpawnerActor::AZombieSpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxZombieCount = 0;
	ZombieCount = 0;
	bCanSpawnZombies = false;
	ZombieSpawnInterval = 0.f;
}

// Called when the game starts or when spawned
void AZombieSpawnerActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AZombieSpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Start spawning zombies every few seconds until boolean is set to false.
	if (bCanSpawnZombies)
	{
		ZombieSpawnInterval += DeltaTime;
		//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, FString::Printf(TEXT("Zombie Interval: %f"), ZombieSpawnInterval));

		if (ZombieSpawnInterval >= 10.0f)
		{
			SpawnZombie();
			ZombieSpawnInterval = 0.f;
		}
	}
}

void AZombieSpawnerActor::SpawnZombie()
{
	if (ZombieAI)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<AZombieAICharacter>(ZombieAI, GetActorTransform(), SpawnParams);
		ZombieCount++;
	}
}

void AZombieSpawnerActor::SetZombieCount(int amount)
{
	ZombieCount = amount;
}

void AZombieSpawnerActor::SetCanSpawnZombies(bool statement)
{
	bCanSpawnZombies = statement;
}

void AZombieSpawnerActor::SetZombieSpawnInterval(float amount)
{
	ZombieSpawnInterval = amount;
}

int AZombieSpawnerActor::GetZombieCount()
{
	return ZombieCount;
}

bool AZombieSpawnerActor::GetCanSpawnZombies()
{
	return bCanSpawnZombies;
}

float AZombieSpawnerActor::GetZombieSpawnInterval()
{
	return ZombieSpawnInterval;
}
