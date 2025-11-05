// Fill out your copyright notice in the Description page of Project Settings.


#include "SprayAmmoProjectileActor.h"
#include "ZombieAICharacter.h"
#include "ZombieShooterCharacter.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet\GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

ASprayAmmoProjectileActor::ASprayAmmoProjectileActor()
{
	//Set both collision and static mesh component to block everything apart from the projectile. 
	//In the child blueprint class the projectile channel is set to ignore.
	CollisionComp->SetCollisionProfileName("Custom");
	CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComp->OnComponentHit.AddDynamic(this, &ASprayAmmoProjectileActor::OnHit);

	StaticMeshComp->SetCollisionProfileName("Custom");
	StaticMeshComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	StaticMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	StaticMeshComp->OnComponentHit.AddDynamic(this, &ASprayAmmoProjectileActor::OnHit);

	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;

	//Create Particle Component
	Explosion = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Explosion"));
	Explosion->SetupAttachment(CollisionComp);
	Explosion->bHiddenInGame = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));

	if (Particle.Succeeded())
	{
		Explosion->Template = Particle.Object;		
	}
}

void ASprayAmmoProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(SprayTimer, this, &ASprayAmmoProjectileActor::OnDestroy, 1.0f, false);
}

void ASprayAmmoProjectileActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AZombieAICharacter* ZombieAIRef = Cast<AZombieAICharacter>(OtherActor);
	if (ZombieAIRef)
	{
		//Damage any enemy it passes through
		UGameplayStatics::ApplyDamage(ZombieAIRef, 30.0f, nullptr, nullptr, nullptr);

		//Spawn Projectile
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion->Template, GetActorTransform());

		//try and play the sound if specified
		if (ExplosionSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), 0.2f);
		}		
		Destroy();
	}
	else
	{
		//Spawn Projectile
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion->Template, GetActorTransform());

		//try and play the sound if specified
		if (ExplosionSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), 0.2f);
		}
		Destroy();
	}
}

void ASprayAmmoProjectileActor::OnDestroy()
{
	if (Explosion)
	{		
		//Spawn Projectile
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion->Template, GetActorTransform());

		// try and play the sound if specified
		if (ExplosionSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), 0.2f);
		}
	}
	Destroy();
}
