// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
class AZombieAICharacter;
class UPawnSensingComponent;

UCLASS()
class ZOMBIESHOOTER_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

protected:
	AZombieAIController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	//function runs when AI Controller possess bot 
	virtual void OnPossess(APawn* InPawn) override;

	//Initialse componets events
	virtual void PostInitializeComponents() override;

	//Pawn sensing component detects the Player
	UFUNCTION()
		void OnSeePawn(APawn* OtherPawn);

	//Checks distance between Player and AI to determine if the player is behind or infront of AI and how far away it is
	void CheckDistance();

	//Increase speed and chase player
	void ChasePlayer();

	//components
	//This gives the AI eyes to detect the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPawnSensingComponent* PawnSensing;	

	//variables
	//Reference to the zombie AI character that is going to be controlled by this controller class
	AZombieAICharacter* ZombieAI;

	//Checks if the AI reached the random location
	bool bisRandLoc;

	float PosX;
	float PosY;

	bool bisChasingPlayer;

	//distance form AI to the player
	float DistanceToPlayer;

	class AZombieShooterCharacter* PlayerRef;
};