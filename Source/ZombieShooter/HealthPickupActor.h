// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthPickupActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UTextRenderComponent;
class AZombieShooterCharacter;

UCLASS()
class ZOMBIESHOOTER_API AHealthPickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Static mesh that represents the Health box
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* HealthComp;

	//The text that displays Health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTextRenderComponent* HealthLabel;

	//Player Reference
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		AZombieShooterCharacter* PlayerRef;

	//functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
