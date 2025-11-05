// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet\GameplayStatics.h"

#include "ZombieAICharacter.h"
#include "ZombieShooterCharacter.h"

AZombieAIController::AZombieAIController()
{
	//Create Pawn sensing Component
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SetPeripheralVisionAngle(90.f);
	PawnSensing->HearingThreshold = 1000.f;
	PawnSensing->LOSHearingThreshold = 1100.f;
	PawnSensing->SightRadius = 1000.f;
	PawnSensing->SetSensingInterval(0.5f);

	bisRandLoc = true;
	bisChasingPlayer = false;
	DistanceToPlayer = 0.0f;
}

void AZombieAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<AZombieShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AZombieAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ZombieAI->GetIsIced())
	{
		ZombieAI->GetCharacterMovement()->MaxWalkSpeed = 100.f;
	}
	else
	{
		ZombieAI->GetCharacterMovement()->MaxWalkSpeed = 400.f;
	}

	//If the AI the loses sight of the player then get it to chase the player again.
	if (ZombieAI->GetCharacterMovement()->Velocity == FVector(0.0f) && ZombieAI)
	{
		if (!ZombieAI->bisOverlapping)
		{
			ChasePlayer();
		}
	}
}

void AZombieAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//Get reference to the pawn of this controller class
	ZombieAI = Cast<AZombieAICharacter>(InPawn);
}

void AZombieAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensing->OnSeePawn.AddDynamic(this, &AZombieAIController::OnSeePawn);
}

void AZombieAIController::OnSeePawn(APawn* OtherPawn)
{
	bisChasingPlayer = true;
	ChasePlayer();
}

void AZombieAIController::CheckDistance()
{
	//Get Distance from AI to player
	DistanceToPlayer = ZombieAI->GetDistanceTo(PlayerRef);

	//Get Dot Product between AI and Player
	if (ZombieAI->GetDotProductTo(PlayerRef) > 0)
	{
		//Postive, player is infront of AI
		DistanceToPlayer *= 1;
	}
	else
	{
		//Negative, player is behind AI
		DistanceToPlayer *= -1;
	}

	//if the AI loses sight of the plaayer then tell it to continue chasing the player
	if (DistanceToPlayer > PawnSensing->SightRadius || DistanceToPlayer < 0)
	{
		MoveToActor(PlayerRef, 5.0f);
	}
}

void AZombieAIController::ChasePlayer()
{	
	MoveToActor(PlayerRef, 5.f);
}
