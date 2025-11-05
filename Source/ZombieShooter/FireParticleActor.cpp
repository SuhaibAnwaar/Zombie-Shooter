// Fill out your copyright notice in the Description page of Project Settings.


#include "FireParticleActor.h"
#include "ZombieAICharacter.h"

#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFireParticleActor::AFireParticleActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Sphere Collision
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetSphereRadius(100.f);
	SphereCollision->SetCollisionProfileName("Custom");
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	//Sphere collision overlap events
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AFireParticleActor::OnOverlapBegin);

	//Create Particle Component
	Fire = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fire"));
	Fire->SetupAttachment(SphereCollision);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));

	if (Particle.Succeeded())
	{
		Fire->Template = Particle.Object;
	}

	bisOverlapping = false;
	DamageTimer = 0.0f;
}

// Called when the game starts or when spawned
void AFireParticleActor::BeginPlay()
{
	Super::BeginPlay();
	
	//die after 3 seconds
	FTimerHandle timer;
	GetWorldTimerManager().SetTimer(timer, this, &AFireParticleActor::Die, 3.0f);
}

// Called every frame
void AFireParticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bisOverlapping)
	{
		//Increment damage timer while the fire particle is overlapping the zombie
		DamageTimer += DeltaTime;

		if (DamageTimer > 0.2f)
		{
			//Reset Damage timer
			DamageTimer = 0.f;
			if (Zombie)
			{
				//Apply Damage
				UGameplayStatics::ApplyDamage(Zombie, 10.0f, nullptr, nullptr, nullptr);
			}
		}
	}
}

void AFireParticleActor::Die()
{
	Destroy();
}

void AFireParticleActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AZombieAICharacter* ZombieAIRef = Cast<AZombieAICharacter>(OtherActor);
	if (ZombieAIRef)
	{
		Zombie = Cast<AZombieAICharacter>(ZombieAIRef);
		if (!bisOverlapping)
		{
			if (Zombie)
			{
				AttachToActor(ZombieAIRef, FAttachmentTransformRules::KeepWorldTransform);
				bisOverlapping = true;
			}
		}
	}
}
