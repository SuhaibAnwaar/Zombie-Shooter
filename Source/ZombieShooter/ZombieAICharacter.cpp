// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAICharacter.h"
#include "ZombieAIWidget.h"
#include "ZombieShooterCharacter.h"
#include "FireAmmoPickupActor.h"
#include "IceAmmoPickupActor.h"
#include "OriginalAmmoPickupActor.h"
#include "PhaseAmmoPickupActor.h"
#include "SprayAmmoPickupActor.h"
#include "HUDUserWidget.h"

#include "Components\CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet\GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Animation/AnimInstance.h"
#include "Components/AudioComponent.h"

// Sets default values
AZombieAICharacter::AZombieAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComp->SetupAttachment(RootComponent);
	WidgetComp->SetRelativeScale3D(FVector(0.1f));

	DamageBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
	DamageBoxComp->SetupAttachment(RootComponent);
	DamageBoxComp->SetCollisionProfileName(FName("Custom"));
	DamageBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DamageBoxComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	DamageBoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	DamageBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AZombieAICharacter::OnOverlapBegin);
	DamageBoxComp->OnComponentEndOverlap.AddDynamic(this, &AZombieAICharacter::OnOverlapEnd);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComp->SetupAttachment(RootComponent);
	AudioComp->VolumeMultiplier = 0.1f;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//zombie faces direction of acceleration
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//rotation speed
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

	DefaultHealth = 100.0f;
	Health = DefaultHealth;
	bisDead = false;
	bCanDamagePlayer = false;
	bisOverlapping = false;
	bisIced = false;
}

// Called when the game starts or when spawned
void AZombieAICharacter::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AZombieShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Player)
	{
		//keep track of how many zombies there are in the round.
		Player->SetRoundZombieCount((Player->GetRoundZombieCount() + 1));
	}

	//Increase the health as rounds move on
	DefaultHealth += Player->GetRound() * 2.f;
	Health = DefaultHealth;

	//Get reference to the widget class being used so that we can manipulate it
	ZombieWidget = Cast<UZombieAIWidget>(WidgetComp->GetUserWidgetObject());

	if (ZombieWidget)
	{
		ZombieWidget->UpdateHealth(DefaultHealth, DefaultHealth);

		WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComp->SetDrawAtDesiredSize(true);		
	}
	
	AudioComp->Play();
}

// Called every frame
void AZombieAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AZombieAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AZombieAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bisDead)//this condition only gets executed once
	{
		bisDead = true;

		//When the Zombie dies, drop it to the floor.
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
		GetCapsuleComponent()->SetCollisionProfileName(FName("OverlapAll"));
		ZombieWidget->SetVisibility(ESlateVisibility::Hidden);

		if (Player)
		{
			//Update zombie count
			Player->SetRoundZombieCount((Player->GetRoundZombieCount() - 1));

			//Spawn an ammo pickup if the timer reaches a certain limit.
			if (Player->GetAmmoSpawnTimer() >= 10.f)
			{
				SpawnAmmo();
				Player->SetAmmoSpawnTimer(0.0f);
			}

			//Stop updating the player's score when it excedes 9999 as there is not enough space to print the number on the HUD.
			if (Player->GetScore() >= 9999)
			{
				Player->SetScore(9999);
				Player->HUDWidget->UpdateScore(Player->GetScore());
			}
			else
			{
				Player->SetScore(Player->GetScore() + 10);
				Player->HUDWidget->UpdateScore(Player->GetScore());
			}
		}

		AudioComp->Stop();		

		//destroy it after a few seconds
		FTimerHandle DeadTimer;
		GetWorldTimerManager().SetTimer(DeadTimer, this, &AZombieAICharacter::Dead, 3.0f, false);
	}
	else if (DamageAmount > 0.0f)
	{
		Health -= DamageAmount;
		if (ZombieWidget)
		{
			ZombieWidget->UpdateHealth(DefaultHealth, Health);
		}
	}
	return 0.0f;
}

void AZombieAICharacter::Dead()
{
	Destroy();
}

void AZombieAICharacter::SpawnAmmo()
{
	//Generate random number
	int RandomNum = FMath::RandRange(1, 5);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (OriginalAmmo && FireAmmo && IceAmmo && PhaseAmmo && SprayAmmo)
	{
		//Spawn ammo
		switch (RandomNum)
		{
		case 1:
			GetWorld()->SpawnActor<AOriginalAmmoPickupActor>(OriginalAmmo, GetActorTransform(), SpawnParams);
			break;
		case 2:
			GetWorld()->SpawnActor<AFireAmmoPickupActor>(FireAmmo, GetActorTransform(), SpawnParams);
			break;
		case 3:
			GetWorld()->SpawnActor<AIceAmmoPickupActor>(IceAmmo, GetActorTransform(), SpawnParams);
			break;
		case 4:
			GetWorld()->SpawnActor<APhaseAmmoPickupActor>(PhaseAmmo, GetActorTransform(), SpawnParams);
			break;
		case 5:
			GetWorld()->SpawnActor<ASprayAmmoPickupActor>(SprayAmmo, GetActorTransform(), SpawnParams);
			break;
		default:
			break;
		}
	}
}

void AZombieAICharacter::SetIsIced(bool statement)
{
	bisIced = statement;
}

bool AZombieAICharacter::GetIsIced()
{
	return bisIced;
}

void AZombieAICharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AZombieShooterCharacter* PlayerRef = Cast<AZombieShooterCharacter>(OtherActor);

	if (PlayerRef)
	{
		bisOverlapping = true;
		//The amount of damage the AI will apply.
		float AIDamage = 1.0f;
		//Damage will increase each round until round 30 where it will remain stationary so the zombies are not overpowered.
		if (PlayerRef->GetRound() <= 30)
		{			
			AIDamage = PlayerRef->GetRound();
		}
		else
		{
			AIDamage = 30;
		}

		//Apply damage every few seconds
		DamageTimerDel.BindUFunction(this, FName("ApplyDamageToPlayer"), PlayerRef, AIDamage);
		GetWorldTimerManager().SetTimer(DamageTimer, DamageTimerDel, 1.0f, true);
	}
}

void AZombieAICharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bisOverlapping = false;

	//When the zombie is no longer overlapping with the player then it cannot apply any damage
	GetWorldTimerManager().ClearTimer(DamageTimer);
}

void AZombieAICharacter::ApplyDamageToPlayer(AZombieShooterCharacter* PlayerRef, float AIDamage)
{
	//try and play the punch sound 
	if (PunchSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PunchSound, GetActorLocation(), 0.5f);
	}

	// try and play a punch animation
	if (PunchAnimation != nullptr)
	{
		PlayAnimMontage(PunchAnimation, 1.f);
	}

	UGameplayStatics::ApplyDamage(PlayerRef, AIDamage, nullptr, nullptr, nullptr);
}
