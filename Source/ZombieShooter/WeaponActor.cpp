// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "ZombieShooterCharacter.h"
#include "ZombieShooterProjectile.h"
#include "HUDUserWidget.h"
#include "PhaseAmmoProjectileActor.h"
#include "MultiAmmoProjectileActor.h"
#include "SprayAmmoProjectileActor.h"

#include "Kismet\GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a gun mesh component
	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	GunMeshComp->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	GunMeshComp->bCastDynamicShadow = false;
	GunMeshComp->CastShadow = false;
	GunMeshComp->SetupAttachment(RootComponent);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(GunMeshComp);
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	TimeBetweenShots = 0.0f;
	CurrentAmmoType = 0;
	DefaultAmmo = 120;
	CurrentAmmo = DefaultAmmo;
	DefaultClip = 30;
	CurrentClip = DefaultClip;

	bCanShootIce = true;
	bCanShootFire = true;

	IceCooldown = 3.0f;
	FireCooldown = 3.0f;

	IceRecharge = 0.0f;
	FireRecharge = 0.0f;
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerRef = Cast<AZombieShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	//the starting ammo type will the multi ammo
	AmmoTypeEnum = EAmmoType::MultiAmmo;

	//The starting multi ammo type will be original
	MultiAmmoTypeEnum = EMultiAmmoType::Original;
	DefaultAmmo = AllDefaultAmmos[0];
	CurrentAmmo = AllDefaultAmmos[0];
	DefaultClip = AllDefaultClips[0];
	CurrentClip = AllDefaultClips[0];
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (AmmoTypeEnum)
	{
		case EAmmoType::MultiAmmo:		
			//if the ice or fire projectile was shot then update the recharge bar.
			//the original ammo type does not have a cooldown hence it does not require the recharge bar.
			switch (MultiAmmoTypeEnum)
			{
			case EMultiAmmoType::Original:
				PlayerRef->HUDWidget->RechargeBar->SetVisibility(ESlateVisibility::Hidden);
				break;
			case EMultiAmmoType::Ice:
				//Only show the progess bar if an ice projectile has been shot
				if (bCanShootIce)
				{
					PlayerRef->SetRechargeBarVisibilty(ESlateVisibility::Hidden);
				}
				else
				{
					PlayerRef->SetRechargeBarVisibilty(ESlateVisibility::Visible);
					PlayerRef->UpdateHUDRechargeBar(IceRecharge, IceCooldown);
				}
				break;
			case EMultiAmmoType::Fire:
				//Only show the progess bar if a fire projectile has been shot
				if (bCanShootFire)
				{
					PlayerRef->SetRechargeBarVisibilty(ESlateVisibility::Hidden);
				}
				else
				{
					PlayerRef->SetRechargeBarVisibilty(ESlateVisibility::Visible);
					PlayerRef->UpdateHUDRechargeBar(FireRecharge, FireCooldown);
				}
				break;
			}
			break;
	case EAmmoType::PhaseAmmo:
			PlayerRef->SetRechargeBarVisibilty(ESlateVisibility::Hidden);
			break;
	case EAmmoType::SprayAmmo:
		PlayerRef->SetRechargeBarVisibilty(ESlateVisibility::Hidden);
		break;
	}
}

void AWeaponActor::StartShooting()
{
	switch (AmmoTypeEnum)
	{
	case EAmmoType::MultiAmmo:
		switch (MultiAmmoTypeEnum)
		{
		case EMultiAmmoType::Original:
			//inital shot
			Shoot();

			//repeatedly call the Shoot function to rapdily fire projectiles
			GetWorldTimerManager().SetTimer(ShootingTimer, this, &AWeaponActor::Shoot, TimeBetweenShots, true);
			break;
		case EMultiAmmoType::Ice:
			//After an Ice shot a timer will start that lasts for a few seconds. 
			//A second timer will start and loop to increment a recharge variable. The recharge variable is used to update the progress bar in the HUD widget.
			if (bCanShootIce)
			{
				Shoot();
				bCanShootIce = false;

				IceRecharge = 0.0f;

				GetWorldTimerManager().SetTimer(IceRechargeTimer, this, &AWeaponActor::RechargeIceShot, 0.1f, true);
			}
			break;
		case EMultiAmmoType::Fire:
			//After a Fire shot a timer will start that lasts for a few seconds. 
			//A second timer will start and loop to increment a recharge variable. The recharge variable is used to update the progress bar in the HUD widget.
			if (bCanShootFire)
			{
				Shoot();
				bCanShootFire = false;

				FireRecharge = 0.0f;

				GetWorldTimerManager().SetTimer(FireRechargeTimer, this, &AWeaponActor::RechargeFireShot, 0.1f, true);
			}
			break;
		}
		break;
	case EAmmoType::PhaseAmmo:
		//inital shot
		Shoot();

		//repeatedly call the Shoot function to rapdily fire projectiles
		GetWorldTimerManager().SetTimer(ShootingTimer, this, &AWeaponActor::Shoot, TimeBetweenShots, true);		
		break;
	case EAmmoType::SprayAmmo:
		//inital shot
		Shoot();
		break;
	default:
		break;
	}
}

void AWeaponActor::StopShooting()
{
	//stop timer
	GetWorldTimerManager().ClearTimer(ShootingTimer);
}

void AWeaponActor::Shoot()
{
	//Only shoot if there is bullets in the clip
	if (CurrentClip > 0)
	{
		// try and fire a projectile
		if (MultiProjectileClass != nullptr && PhaseProjectileClass != nullptr && SprayProjectileClass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				FRotator SpawnRotation = PlayerRef->GetControlRotation();

				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				//spawn the correct projectile at the muzzle
				if (AmmoTypeEnum == EAmmoType::MultiAmmo)
				{					
					World->SpawnActor<AMultiAmmoProjectileActor>(MultiProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				}
				else if (AmmoTypeEnum == EAmmoType::PhaseAmmo)
				{
					World->SpawnActor<APhaseAmmoProjectileActor>(PhaseProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				}
				else if (AmmoTypeEnum == EAmmoType::SprayAmmo)
				{				
					float YawRot = SpawnRotation.Yaw;

					for (int i = 0; i < 3; i++)
					{
						if (i == 0)
						{
							YawRot = SpawnRotation.Yaw - 15.f;
						}
						else if (i == 1)
						{
							YawRot = SpawnRotation.Yaw;
						}
						else if (i == 2)
						{
							YawRot = SpawnRotation.Yaw + 15.f;
						}
						SpawnSprayProjectiles(YawRot);
					}
				}
				//take one bullet away from clip magazine every time the player shoots
				CurrentClip -= 1;
			}

			PlayerRef->UpdateHUDWidgets();
			UpdateAmmoTypes();

			// try and play the sound if specified
			if (WeaponSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, WeaponSound, GetActorLocation(), 0.2f);
			}

			// try and play a firing animation if specified
			if (WeaponAnimation != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = PlayerRef->Mesh1P->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(WeaponAnimation, 1.f);
				}
			}
		}
	}
	else
	{
		//stop timer
		GetWorldTimerManager().ClearTimer(ShootingTimer);
	}
}

void AWeaponActor::AmmoSelector(bool bWheelUp)
{
	switch (AmmoTypeEnum)//When switching ammo types, make sure the correct amount of ammo and clip size is allocated.
	{
	case EAmmoType::MultiAmmo:

		//if this function is called by the mouse wheel up event then decrement through the ammo types
		if (bWheelUp)
		{
			CurrentAmmoType -= 1;
		}
		else//else the function is called by the mouse wheel down event, so increment through the ammo types
		{
			CurrentAmmoType += 1;
		}

		//when the index reaches an outer limit, reset the index to an inner limit
		if (CurrentAmmoType == 3)//scrolling up
		{
			CurrentAmmoType = 0;
		}
		else if (CurrentAmmoType == -1)//scrolling down
		{
			CurrentAmmoType = 2;
		}

		switch (CurrentAmmoType)
		{
		case 0: MultiAmmoTypeEnum = EMultiAmmoType::Original;
			DefaultAmmo = AllDefaultAmmos[0];
			CurrentAmmo = AllCurrentAmmos[0];
			DefaultClip = AllDefaultClips[0];
			CurrentClip = AllCurrentClips[0];
			break;
		case 1: MultiAmmoTypeEnum = EMultiAmmoType::Ice;
			DefaultAmmo = AllDefaultAmmos[1];
			CurrentAmmo = AllCurrentAmmos[1];
			DefaultClip = AllDefaultClips[1];
			CurrentClip = AllCurrentClips[1];
			break;
		case 2: MultiAmmoTypeEnum = EMultiAmmoType::Fire;
			DefaultAmmo = AllDefaultAmmos[2];
			CurrentAmmo = AllCurrentAmmos[2];
			DefaultClip = AllDefaultClips[2];
			CurrentClip = AllCurrentClips[2];
			break;
		default:
			break;
		}

		break;
	case EAmmoType::PhaseAmmo:
		DefaultAmmo = AllDefaultAmmos[3];
		CurrentAmmo = AllCurrentAmmos[3];
		DefaultClip = AllDefaultClips[3];
		CurrentClip = AllCurrentClips[3];
		break;
	case EAmmoType::SprayAmmo:
		DefaultAmmo = AllDefaultAmmos[4];
		CurrentAmmo = AllCurrentAmmos[4];
		DefaultClip = AllDefaultClips[4];
		CurrentClip = AllCurrentClips[4];
	default:
		break;
	}

	PlayerRef->UpdateHUDWidgets();
}

void AWeaponActor::Reloading()
{
	//if there is enough ammo, then reload weapon
	if (CurrentAmmo >= DefaultClip)
	{
		//Only reload if the player has fired at least one bullet
		if (CurrentClip < DefaultClip)
		{
			CurrentClip = DefaultClip;
			CurrentAmmo -= DefaultClip;

			PlayerRef->UpdateHUDWidgets();
			UpdateAmmoTypes();
		}
	}
}

void AWeaponActor::UpdateAmmoTypes()
{
	switch(AmmoTypeEnum)	
	{
	case EAmmoType::MultiAmmo:
		switch (MultiAmmoTypeEnum)
		{
		case EMultiAmmoType::Original:
			AllCurrentAmmos[0] = CurrentAmmo;
			AllCurrentClips[0] = CurrentClip;
			break;
		case EMultiAmmoType::Ice:
			AllCurrentAmmos[1] = CurrentAmmo;
			AllCurrentClips[1] = CurrentClip;
			break;
		case EMultiAmmoType::Fire:
			AllCurrentAmmos[2] = CurrentAmmo;
			AllCurrentClips[2] = CurrentClip;
			break;
		}
		break;
	case EAmmoType::PhaseAmmo:
		AllCurrentAmmos[3] = CurrentAmmo;
		AllCurrentClips[3] = CurrentClip;
		break;
	case EAmmoType::SprayAmmo:
		AllCurrentAmmos[4] = CurrentAmmo;
		AllCurrentClips[4] = CurrentClip;
		break;
	}	
}

void AWeaponActor::RechargeIceShot()
{
	IceRecharge += 0.1f;

	if (IceRecharge >= IceCooldown)
	{
		bCanShootIce = true;
		GetWorldTimerManager().ClearTimer(IceRechargeTimer);
		PlayerRef->HUDWidget->RechargeBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AWeaponActor::RechargeFireShot()
{
	FireRecharge += 0.1f;

	if (FireRecharge >= FireCooldown)
	{
		bCanShootFire = true;
		GetWorldTimerManager().ClearTimer(FireRechargeTimer);
		PlayerRef->HUDWidget->RechargeBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AWeaponActor::SpawnSprayProjectiles(float YawRot)
{
	FRotator SpawnRotation = PlayerRef->GetControlRotation();
	SpawnRotation.Yaw = YawRot;

	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<ASprayAmmoProjectileActor>(SprayProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
}

void AWeaponActor::SetAmmoTypeEnum(EAmmoType ammotype)
{
	AmmoTypeEnum = ammotype;
}

void AWeaponActor::SetMultiAmmoTypeEnum(EMultiAmmoType multiammotype)
{
	MultiAmmoTypeEnum = multiammotype;
}

EAmmoType AWeaponActor::GetCurrentAmmoType()
{
	return AmmoTypeEnum;
}

EMultiAmmoType AWeaponActor::GetCurrentMultiAmmoType()
{
	return MultiAmmoTypeEnum;
}
