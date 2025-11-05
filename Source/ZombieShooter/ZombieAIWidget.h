// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZombieAIWidget.generated.h"

/**
 * 
 */
class UProgressBar;

UCLASS()
class ZOMBIESHOOTER_API UZombieAIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void UpdateHealth(float DefaultHealth, float Health);

	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBar;
};
