// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieShooterGameMode.h"
#include "ZombieShooterHUD.h"
#include "ZombieShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "ZombieShooterCharacter.h"
#include "ZombieAICharacter.h"

AZombieShooterGameMode::AZombieShooterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AZombieShooterHUD::StaticClass();
}
