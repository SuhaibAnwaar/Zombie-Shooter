// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

class AZombieShooterCharacter;
class USkeletalMeshComponent;
class UAnimMontage;
class USoundBase;
class UPawnNoiseEmitterComponent;

//All the ammo types the player can use
UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	MultiAmmo UMETA(DisplayName = "MultiAmmo"),
	PhaseAmmo UMETA(DisplayName = "PhaseAmmo"),
	SprayAmmo UMETA(DisplayName = "SprayAmmo"),
};

//All the multi ammo types
UENUM(BlueprintType)
enum class EMultiAmmoType : uint8
{
	Original UMETA(DisplayName = "Original"),
	Ice UMETA(DisplayName = "Ice"),
	Fire UMETA(DisplayName = "Fire"),
};

UCLASS()
class ZOMBIESHOOTER_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
private:	
	//enum holds all the ammo types
	TEnumAsByte<EAmmoType> AmmoTypeEnum;

	//enum holds all the multi ammo types
	TEnumAsByte<EMultiAmmoType> MultiAmmoTypeEnum;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* GunMeshComp;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USceneComponent* MuzzleLocation;
public:	
	// Sets default values for this actor's properties
	AWeaponActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Calls OnShoot function repeatdly
	void StartShooting();

	//Stops Calling OnShoot function
	void StopShooting();

	//Fires a projectile.
	void Shoot();

	//Changes players current ammo type
	void AmmoSelector(bool bWheelUp);

	//Reload Weapon
	void Reloading();

	//each ammo type is updated with the correct amount of ammo and bullets in the clip.
	void UpdateAmmoTypes();

	//Recharge the ice shot
	void RechargeIceShot();

	//Recharge the fire shot
	void RechargeFireShot();

	//Spawn the spray projectile.
	void SpawnSprayProjectiles(float YawRot);

	//Setters
	void SetAmmoTypeEnum(EAmmoType ammotype);
	void SetMultiAmmoTypeEnum(EMultiAmmoType multiammotype);

	//Getters
	EAmmoType GetCurrentAmmoType();
	EMultiAmmoType GetCurrentMultiAmmoType();

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* WeaponSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* WeaponAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	//Multi Projectile
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AMultiAmmoProjectileActor> MultiProjectileClass;

	//Phase Projectile
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class APhaseAmmoProjectileActor> PhaseProjectileClass;

	//Spray Projectile
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ASprayAmmoProjectileActor> SprayProjectileClass;

	AZombieShooterCharacter* PlayerRef;

	//the maximum amount of ammo the player can have
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int DefaultAmmo;

	//how much ammo the player has remaining.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int CurrentAmmo;

	//the maximum amount of bullets that can be in one clip.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int DefaultClip;

	//how many bullets remain in the clip.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int CurrentClip;

	//the number of seconds between shots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float TimeBetweenShots;

	//the maximum amount of ammo allowed for each ammo type. 
	//0 = original, 1 = Ice, 2 = Fire,  3 = Phase, 4 = Spray
	int AllDefaultAmmos[5] = { 120, 20, 20, 60, 80 };
	//how much ammo remaining for each ammo type.
	int AllCurrentAmmos[5] = { AllDefaultAmmos[0], AllDefaultAmmos[1], AllDefaultAmmos[2], 0, 0 };

	//the maximum amount of bullets that can be in the clip for each ammo type.
	//0 = orginal, 1 = Ice, 2 = Fire, 3 = Phase
	int AllDefaultClips[5] = { 30, 10, 10, 20, 10 };
	//how many bullets left in the clip for each ammo type
	int AllCurrentClips[5] = { AllDefaultClips[0], AllDefaultClips[1], AllDefaultClips[2], 0, 0 };

private:
	//timer handle for firing a projectile
	FTimerHandle ShootingTimer;
	//timer delegate for firing a projectile
	FTimerDelegate ShootingTimerDel;

	//Timer handle for firing three spray projectiles
	FTimerHandle SprayAmmoTimer;
	//Timer delegate for firing three spray projectiles
	FTimerDelegate SprayAmmoTimerDel;

	//index for the ammo types
	//0 = orginal, 1 = Ice, 2 = Fire, 3 = Phase
	int CurrentAmmoType;

	//can the player shoot with this projectile 
	bool bCanShootIce;
	bool bCanShootFire;

	//How long it takes for the projectile to fire again
	float IceCooldown;
	float FireCooldown;

	//float timers that allow when the ice and fire projectiles can be shot again and updates their recharge bar in the HUD widget.
	float IceRecharge;
	float FireRecharge;

	//calls a the recharge function repeatdly.
	FTimerHandle IceRechargeTimer;
	FTimerHandle FireRechargeTimer;
};
