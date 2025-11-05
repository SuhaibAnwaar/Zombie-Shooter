// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBoxActor.h"
#include "ZombieShooterCharacter.h"
#include "WeaponActor.h"
#include "HUDUserWidget.h"

#include "Components/TextRenderComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAmmoBoxActor::AAmmoBoxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Static Mesh
	AmmoComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoComp"));
	AmmoComp->SetupAttachment(RootComponent);
	AmmoComp->SetSimulatePhysics(false);
	AmmoComp->SetCollisionProfileName(TEXT("OverlapAll"));
	AmmoComp->SetWorldScale3D(FVector(.5f, 0.5f, 0.5f));

	//Create Ammo Text
	AmmoLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("AmmoLabel"));
	AmmoLabel->SetupAttachment(AmmoComp);
	AmmoLabel->SetText(TEXT("Ammo"));
	AmmoLabel->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	AmmoLabel->SetRelativeLocation(FVector(0.0f, 0.f, 43.f));
	AmmoLabel->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
}

// Called when the game starts or when spawned
void AAmmoBoxActor::BeginPlay()
{
	Super::BeginPlay();
	
	//get reference
	PlayerRef = Cast<AZombieShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AAmmoBoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerRef)
	{
		//how much the Text needs to rotate before facing the player
		FRotator SetLocation_Rot = UKismetMathLibrary::FindLookAtRotation(AmmoLabel->GetComponentLocation(), PlayerRef->GetActorLocation());

		//Only Rotate on the Z Axis
		AmmoLabel->SetWorldRotation(FRotator(AmmoLabel->GetRelativeRotation().Pitch, SetLocation_Rot.Yaw, AmmoLabel->GetRelativeRotation().Roll));
	}
}

void AAmmoBoxActor::UpdateCurrentAmmo()
{
	//If Ammo selected is below DefaultAmmo limit add one clip size to the ammo but clamp the Ammo
	//so the bullets added does not go over the max Ammo limit
	//then destroy AmmoBox
	if (PlayerRef->Weapon->CurrentAmmo < PlayerRef->Weapon->DefaultAmmo)
	{
		PlayerRef->Weapon->CurrentAmmo = FMath::Clamp(PlayerRef->Weapon->CurrentAmmo + PlayerRef->Weapon->DefaultClip, 0, PlayerRef->Weapon->DefaultAmmo);

		//if the magazine clip is empty then add ammo to the magazine clip
		if (PlayerRef->Weapon->CurrentClip == 0)
		{
			PlayerRef->Weapon->CurrentClip = PlayerRef->Weapon->DefaultClip;
			PlayerRef->Weapon->CurrentAmmo -= PlayerRef->Weapon->DefaultClip;
		}
		PlayerRef->HUDWidget->UpdateAmmo();
		PlayerRef->Weapon->UpdateAmmoTypes();
		Destroy();
	}
}

void AAmmoBoxActor::UpdateLastAmmo(int AmmoIndex)
{
	//If Ammo is not selected is below DefaultAmmo limit add one clip size to the ammo but clamp the Ammo
	//so the bullets added does not go over the max Ammo limit
	//then destroy AmmoBox
	if (PlayerRef->Weapon->AllCurrentAmmos[AmmoIndex] < PlayerRef->Weapon->AllDefaultAmmos[AmmoIndex])
	{
		PlayerRef->Weapon->AllCurrentAmmos[AmmoIndex] = FMath::Clamp(PlayerRef->Weapon->AllCurrentAmmos[AmmoIndex] + PlayerRef->Weapon->AllDefaultClips[AmmoIndex], 0, PlayerRef->Weapon->AllDefaultAmmos[AmmoIndex]);

		//if the magazine clip is empty then add ammo to the magazine clip
		if (PlayerRef->Weapon->AllCurrentClips[AmmoIndex] == 0)
		{
			PlayerRef->Weapon->AllCurrentClips[AmmoIndex] = PlayerRef->Weapon->AllDefaultClips[AmmoIndex];
			PlayerRef->Weapon->AllCurrentAmmos[AmmoIndex] -= PlayerRef->Weapon->AllDefaultClips[AmmoIndex];
		}
		Destroy();
	}
}

