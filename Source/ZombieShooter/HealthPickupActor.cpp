// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickupActor.h"
#include "HUDUserWidget.h"

#include "ZombieShooterCharacter.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHealthPickupActor::AHealthPickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Static mesh
	HealthComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthComp"));
	HealthComp->SetupAttachment(RootComponent);
	HealthComp->SetSimulatePhysics(false);
	HealthComp->SetRelativeScale3D(FVector(0.5f));
	HealthComp->SetCollisionProfileName(TEXT("OverlapAll"));
	//Call overlap function when the player overlaps the static mesh
	HealthComp->OnComponentBeginOverlap.AddDynamic(this, &AHealthPickupActor::OnOverlapBegin);

	//Create Health Label
	HealthLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthLabel"));
	HealthLabel->SetupAttachment(HealthComp);
	HealthLabel->SetText(TEXT("Health"));
	HealthLabel->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	HealthLabel->SetRelativeLocation(FVector(0.0f, 0.f, 37.877163f));
	HealthLabel->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
}

// Called when the game starts or when spawned
void AHealthPickupActor::BeginPlay()
{
	Super::BeginPlay();

	//get reference
	PlayerRef = Cast<AZombieShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AHealthPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerRef)
	{
		//how much the Text needs to rotate before facing the player
		FRotator SetLocation_Rot = UKismetMathLibrary::FindLookAtRotation(HealthLabel->GetComponentLocation(), PlayerRef->GetActorLocation());

		//Only Rotate on the Z Axis
		HealthLabel->SetWorldRotation(FRotator(HealthLabel->GetRelativeRotation().Pitch, SetLocation_Rot.Yaw, HealthLabel->GetRelativeRotation().Roll));
	}
}

void AHealthPickupActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (PlayerRef == OtherActor)
	{
		if (PlayerRef->Health < PlayerRef->DefaultHealth)
		{
			//Update Players Health but make sure it does not exceed the max health limit
			PlayerRef->Health = FMath::Clamp(PlayerRef->Health + 20, 0.0f, PlayerRef->DefaultHealth);

			if (PlayerRef->HUDWidget)
			{
				PlayerRef->HUDWidget->UpdateHealth(PlayerRef->DefaultHealth, PlayerRef->Health);
			}
			Destroy();
		}
	}
}

