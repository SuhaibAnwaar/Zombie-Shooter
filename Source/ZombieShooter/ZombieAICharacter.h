// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieAICharacter.generated.h"

class UZombieAIWidget;
class UUserWidget;

UCLASS()
class ZOMBIESHOOTER_API AZombieAICharacter : public ACharacter
{
	GENERATED_BODY()

private:
	//true means the zombie has been shot with the ice ammo.
	bool bisIced;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Widget component will hold the health bar
	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* WidgetComp;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* DamageBoxComp;

	UPROPERTY(VisibleAnywhere)
		class UAudioComponent* AudioComp;

	// declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void ApplyDamageToPlayer(class AZombieShooterCharacter* PlayerRef, float AIDamage);
public:	
	// Sets default values for this character's properties
	AZombieAICharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Receives Damage
	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Dead();

	void SpawnAmmo();
	
	//Setters
	void SetIsIced(bool statement);

	//Getters
	bool GetIsIced();

	//Variables exposed to blueprints
	UPROPERTY(EditAnywhere, Category = "UI")
		float DefaultHealth;

	UPROPERTY(EditAnywhere, Category = "UI")
		float Health;

	//Initial Hearing for enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
		float InitialPawnHearingThreshold;

	//Update hearing for when the player enters the same level as enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
		float UpdatedPawnHearingThreshold;

	//Initial Sight Radius
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
		float InitialSightRadius;

	//how far the enemy can see. This is used used so enemies in different position can detect the enemy more easily and therefore shoot standing further away
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
		float UpdatedSightRadius;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AFireAmmoPickupActor> FireAmmo;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AIceAmmoPickupActor> IceAmmo;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AOriginalAmmoPickupActor> OriginalAmmo;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class APhaseAmmoPickupActor> PhaseAmmo;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ASprayAmmoPickupActor> SprayAmmo;

	UPROPERTY(EditDefaultsOnly)
		class UAnimMontage* PunchAnimation;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* PunchSound;

	//So we can manipulate the class after casting to using the WidgetRef variable.
	UZombieAIWidget* ZombieWidget;

	bool bisDead;

	bool bCanDamagePlayer;

	bool bisOverlapping;

	FTimerHandle DamageTimer;
	//Timer delegate handles passing parameters when setting a timer
	FTimerDelegate DamageTimerDel;

	class AZombieShooterCharacter* Player;
};
