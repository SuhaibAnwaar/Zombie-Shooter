// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieShooterCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class USoundBase;
class UUserWidget;
class UHUDUserWidget;

UCLASS(config=Game)
class AZombieShooterCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	AZombieShooterCharacter();

	//the number of zombies left in the round.
	int RoundZombieCount;

	//stores which round the player is currently on.
	int Round;

	//When ammo can spawn from a dead zombie
	float AmmoSpawnTimer;

	//Player Score
	int Score;
protected:
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime);
public:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* Mesh1P;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//Reference to widget class for the player
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UUserWidget> WidgetClass;

	//The HUD that will be used for the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		UHUDUserWidget* HUDWidget;

	//Reference to widget class for the player
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UUserWidget> PauseWidgetClass;

	//The HUD that will be used for the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		class UPauseWidget* PauseWidget;

	//Reference to widget class for the player
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UUserWidget> GameOverWidgetClass;

	//The HUD that will be used for the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		class UGameOverWidget* GameOverWidget;

	//Used to get reference to all the zombies spawners in the game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	TArray<class AZombieSpawnerActor*> ZombieSpawnerRef;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeaponActor> WeaponClass;
	
	UPROPERTY(EditDefaultsOnly)
		USoundBase* GameMusic;

	class AWeaponActor* Weapon;

	//Setters
	void SetRoundZombieCount(int amount);
	void SetRound(int amount);
	void SetAmmoSpawnTimer(float amount);
	void SetScore(int amount);

	//Getters
	int GetRoundZombieCount();
	int GetRound();
	int GetScore();
	float GetAmmoSpawnTimer();

	//max health
	float DefaultHealth;

	//current health
	float Health;

	//for how long zombies spawn in each round
	float ZombieSpawnTimer;

	//How long the timer can go for.
	float MaxZombieSpawnTimer;

	//The timer inbetween each round
	float IntervalTimer;

	//How long game music lasts for.
	float GameMusicTimer;

	//How long the zombie sounds plays for.
	float ZombieSoundTimer;

	//When the zombies can start spawning
	bool bCanSpawnZombie;

	//true means the interval timer can start counting.
	bool bCanIntervalTimerBegin;
protected:	
	//Calls StartShooting function from the Weapon class
	void StartShooting();

	//Calls StopShooting function from the Weapon class
	void StopShooting();

	///Calls AmmoSelector function from the Weapon class
	void AmmoSelector(bool bWheelUp);

	//Calls Reloading function from the Weapon class
	void Reloading();

	//Receives Damage
	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void AmmoSelection(const int InItemIndex);

	void Pause();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	//Update the Ammo and Ammo images in the player HUD.
	//This function is usually called by other classes, mainly the weapon class.
	void UpdateHUDWidgets();

	//Update the the percent on the progress bar
	void UpdateHUDRechargeBar(float recharge, float cooldown);

	//sets the visibility of the progress bar
	void SetRechargeBarVisibilty(ESlateVisibility visibility);
 };