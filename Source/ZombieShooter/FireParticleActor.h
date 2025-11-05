// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireParticleActor.generated.h"

class UParticleSystemComponent;
class USphereComponent;

UCLASS()
class ZOMBIESHOOTER_API AFireParticleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireParticleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bisOverlapping;

	//How often fire damages the zombie
	float DamageTimer;

	//Used to damage the zombie
	class AZombieAICharacter* Zombie;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Fire Particle
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UParticleSystemComponent* Fire;

	//Used for overlap events so the fire can damage the AI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* SphereCollision;

	//Destroy self
	void Die();

	//Functions 
	//declare Sphere Component overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
