// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieShooterCharacter.h"
#include "HUDUserWidget.h"
#include "ZombieShooterProjectile.h"
#include "WeaponActor.h"
#include "ZombieSpawnerActor.h"
#include "PauseWidget.h"
#include "GameOverWidget.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AZombieShooterCharacter

AZombieShooterCharacter::AZombieShooterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	DefaultHealth = 100.f;
	Health = DefaultHealth;
	MaxZombieSpawnTimer = 40.f;
	ZombieSpawnTimer = 0.0f;
	Round = 1;
	RoundZombieCount = 0;
	IntervalTimer = 0.0f;
	AmmoSpawnTimer = 0.0f;
	GameMusicTimer = 0.0f;
	ZombieSoundTimer = 0.0f;
	bCanSpawnZombie = true;
	bCanIntervalTimerBegin = false;
}

void AZombieShooterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Spawn player's weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform WeaponTransform;
	WeaponTransform.SetLocation(FVector::ZeroVector);
	WeaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));

	if (WeaponClass)
	{
		Weapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponClass, WeaponTransform, SpawnParams);

		if (Weapon)
		{
			Weapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		}
	}

	Mesh1P->SetHiddenInGame(false, true);

	//Add Widget to viewport
	if (IsValid(WidgetClass))
	{
		HUDWidget = Cast<UHUDUserWidget>(CreateWidget(GetWorld(), WidgetClass));

		if (HUDWidget != nullptr)
		{
			HUDWidget->AddToViewport();
			HUDWidget->UpdateAmmo();
			HUDWidget->UpdateHealth(DefaultHealth, Health);
			HUDWidget->UpdateRoundNumber(Round);
		}
	}

	//try and play the Game Music
	if (GameMusic != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GameMusic, GetActorLocation(), 0.4f);
	}
}

void AZombieShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameMusicTimer < 140.f)
	{
		//Increment timer
		GameMusicTimer += DeltaTime;
	}
	else
	{
		//Reset Timer
		GameMusicTimer = 0.0f;

		//try and play the Game Music
		if (GameMusic != nullptr)
		{
			//Restart Game Music
			UGameplayStatics::PlaySoundAtLocation(this, GameMusic, GetActorLocation(), 0.4f);
		}
	}

	//Tell all the zombie spawner classes to start spawning zombies
	if (bCanSpawnZombie)
	{	
		bCanSpawnZombie = false;

		if (ZombieSpawnerRef.Num() != 0)
		{
			for (int i = 0; i < ZombieSpawnerRef.Num(); i++)
			{
				AZombieSpawnerActor* SpawnerRef = Cast<AZombieSpawnerActor>(ZombieSpawnerRef[i]);

				if (SpawnerRef)
				{
					SpawnerRef->SetZombieCount(0);
					SpawnerRef->SetCanSpawnZombies(true);	
				}
			}
		}
	}

	if (ZombieSpawnTimer <= MaxZombieSpawnTimer)
	{
		//Increase timer
		ZombieSpawnTimer += DeltaTime;
		//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, FString::Printf(TEXT("Zombie Spawn Timer: %f"), ZombieSpawnTimer));

		//Used by the zombie class
		AmmoSpawnTimer += DeltaTime;
		//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, FString::Printf(TEXT("Ammo Timer: %f"), AmmoSpawnTimer));
	}	
	else if (ZombieSpawnTimer > MaxZombieSpawnTimer) //When timer reaches the limit then stop spawning zombies.
	{
		for (int i = 0; i < ZombieSpawnerRef.Num(); i++)
		{
			AZombieSpawnerActor* SpawnerRef = Cast<AZombieSpawnerActor>(ZombieSpawnerRef[i]);

			SpawnerRef->SetCanSpawnZombies(false);
			SpawnerRef->SetZombieSpawnInterval(0.f);	 
		}		

		//When there is no more zombies left in the arena start preparation for the next round.
		if (RoundZombieCount <= 0)
		{
			//Start interval timer.
			bCanIntervalTimerBegin = true;

			//Reset Round Count
			RoundZombieCount = 0;
		}
	}

	if (bCanIntervalTimerBegin)
	{
		//Increase interval timer until it reaches 5 seconds
		IntervalTimer += DeltaTime;

		if (HUDWidget)
		{
			HUDWidget->EndOfRoundText->SetVisibility(ESlateVisibility::Visible);
		}

		//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, FString::Printf(TEXT("Interval Timer: %f"), IntervalTimer));
		if (IntervalTimer >= 5.f)
		{
			//Reset booleans so the next round can start.
			bCanIntervalTimerBegin = false;
			bCanSpawnZombie = true;

			//Increase the length of time zombies have to spawn in the next round 
			MaxZombieSpawnTimer = ZombieSpawnTimer + 5.f;
			//Reset spawn timer
			ZombieSpawnTimer = 0.f;			
			//Reset interval timer
			IntervalTimer = 0.0f;

			//Increase round count
			Round++;
			
			if (HUDWidget)
			{
				HUDWidget->UpdateRoundNumber(Round);
				HUDWidget->EndOfRoundText->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AZombieShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AZombieShooterCharacter::StartShooting);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AZombieShooterCharacter::StopShooting);

	//Bind reload event
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AZombieShooterCharacter::Reloading);

	//Bind mouse wheel events
	DECLARE_DELEGATE_OneParam(FCustomInputDelegate, const bool);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("MouseWheelUp", IE_Pressed, this, &AZombieShooterCharacter::AmmoSelector, true);
	
	DECLARE_DELEGATE_OneParam(FCustomInputDelegate, const bool);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("MouseWheelDown", IE_Pressed, this, &AZombieShooterCharacter::AmmoSelector, false);

	//bind inventory events
	DECLARE_DELEGATE_OneParam(FInputSwitchInventoryDelegate, const int);
	PlayerInputComponent->BindAction<FInputSwitchInventoryDelegate>("InventoryOne", IE_Pressed, this, &AZombieShooterCharacter::AmmoSelection, 1); //Multi Ammo

	DECLARE_DELEGATE_OneParam(FInputSwitchInventoryDelegate, const int);
	PlayerInputComponent->BindAction<FInputSwitchInventoryDelegate>("InventoryTwo", IE_Pressed, this, &AZombieShooterCharacter::AmmoSelection, 2); //Phase Ammo

	DECLARE_DELEGATE_OneParam(FInputSwitchInventoryDelegate, const int);
	PlayerInputComponent->BindAction<FInputSwitchInventoryDelegate>("InventoryThree", IE_Pressed, this, &AZombieShooterCharacter::AmmoSelection, 3); //Phase Ammo

	//Pause Menu
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AZombieShooterCharacter::Pause);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AZombieShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AZombieShooterCharacter::MoveRight);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AZombieShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AZombieShooterCharacter::LookUpAtRate);
}

void AZombieShooterCharacter::SetRoundZombieCount(int amount)
{
	RoundZombieCount = amount;
}

void AZombieShooterCharacter::SetRound(int amount)
{
	Round = amount;
}

void AZombieShooterCharacter::SetAmmoSpawnTimer(float amount)
{
	AmmoSpawnTimer = amount;
}

void AZombieShooterCharacter::SetScore(int amount)
{
	Score = amount;
}

int AZombieShooterCharacter::GetRoundZombieCount()
{
	return RoundZombieCount;
}

int AZombieShooterCharacter::GetRound()
{
	return Round;
}

int AZombieShooterCharacter::GetScore()
{
	return Score;
}

float AZombieShooterCharacter::GetAmmoSpawnTimer()
{
	return AmmoSpawnTimer;
}

void AZombieShooterCharacter::StartShooting()
{
	Weapon->StartShooting();
}

void AZombieShooterCharacter::StopShooting()
{
	Weapon->StopShooting();
}

void AZombieShooterCharacter::AmmoSelector(bool bWheelUp)
{
	Weapon->AmmoSelector(bWheelUp);
}

void AZombieShooterCharacter::Reloading()
{
	Weapon->Reloading();
}

void AZombieShooterCharacter::UpdateHUDWidgets()
{
	HUDWidget->UpdateAmmo();
	HUDWidget->UpdateAmmoImages();
}

void AZombieShooterCharacter::UpdateHUDRechargeBar(float recharge, float cooldown)
{
	HUDWidget->UpdateRechargeBar(recharge, cooldown);
}

void AZombieShooterCharacter::SetRechargeBarVisibilty(ESlateVisibility visibility)
{
	HUDWidget->RechargeBar->SetVisibility(visibility);
}

float AZombieShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health <= 0.0f)
	{
		//Add Widget to viewport
		if (IsValid(GameOverWidgetClass))
		{
			if (HUDWidget)
			{
				HUDWidget->RemoveFromParent();
			}
			GameOverWidget = Cast<UGameOverWidget>(CreateWidget(GetWorld(), GameOverWidgetClass));

			if (GameOverWidget != nullptr)
			{
				GameOverWidget->AddToViewport();
				//set game to pause so they cannot shoot or move around when the game over screen added to the viewport.
				UGameplayStatics::SetGamePaused(GetWorld(), true);

				APlayerController* PlayerController = Cast< APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
				if (PlayerController)
				{
					PlayerController->bShowMouseCursor = true;
					PlayerController->SetInputMode(FInputModeGameAndUI::FInputModeGameAndUI().SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen));
				}
			}
		}
	}
	else if (DamageAmount > 0.0f)
	{
		Health -= DamageAmount;
	}
	
	if (HUDWidget != nullptr)
	{
		//update the health on the widget.
		HUDWidget->UpdateHealth(DefaultHealth, Health);
	}

	return 0.0f;
}

void AZombieShooterCharacter::AmmoSelection(const int InItemIndex)
{		
	switch(InItemIndex)
	{
	case 1:
		if (Weapon)
		{
			Weapon->SetAmmoTypeEnum(EAmmoType::MultiAmmo);
			AmmoSelector(false);
		}
		break;
	case 2:
		if (Weapon)
		{
			Weapon->SetAmmoTypeEnum(EAmmoType::PhaseAmmo);
			AmmoSelector(false);
		}
		break;
	case 3:
		if (Weapon)
		{
			Weapon->SetAmmoTypeEnum(EAmmoType::SprayAmmo);
			AmmoSelector(false);
		}
	}
}

void AZombieShooterCharacter::Pause()
{
	if (!IsValid(PauseWidget))
	{
		//Add Widget to viewport
		if (IsValid(PauseWidgetClass))
		{
			PauseWidget = Cast<UPauseWidget>(CreateWidget(GetWorld(), PauseWidgetClass));
		}
	}	

	//Pause and unpause using keyboard press.
	if (PauseWidget)
	{
		APlayerController* PlayerController = Cast< APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		//Unpause
		if (UGameplayStatics::IsGamePaused(GetWorld()))
		{
			PauseWidget->RemoveFromParent();
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = false;
				PlayerController->SetInputMode(FInputModeGameOnly::FInputModeGameOnly());
			}
		}
		else//pause
		{
			PauseWidget->AddToViewport();
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = true;
				PlayerController->SetInputMode(FInputModeGameAndUI::FInputModeGameAndUI().SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen));
			}
		}
	}
}

void AZombieShooterCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AZombieShooterCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AZombieShooterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AZombieShooterCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
