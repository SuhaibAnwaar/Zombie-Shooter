// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDUserWidget.generated.h"

class UTextBlock;
class UProgressBar;
class AZombieShooterCharacter;
class UImage;
class UTexture2D;

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Ammo;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Divider;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Clip;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* RechargeBar;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* CurrentAmmo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Ammo2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Ammo3;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Round;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* RoundText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ScoreText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ActualScoreText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* EndOfRoundText;

	AZombieShooterCharacter* PlayerRef;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TArray<UTexture2D*> AmmoTex;
	
	//Used when we need to hide certain ammo textures from the HUD
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		class UMaterial* NoAmmoMat;

	UFUNCTION(BlueprintCallable)
		void UpdateHealth(float DefaultHealth, float Health);

	UFUNCTION(BlueprintCallable)
		void UpdateAmmo();

	//Recharge the player's Ice and Fire ammo
	UFUNCTION(BlueprintCallable)
		void UpdateRechargeBar(float Recharge, float MaxRecharge);

	UFUNCTION(BlueprintCallable)
		void UpdateAmmoImages();

	UFUNCTION(BlueprintCallable)
		void UpdateRoundNumber(int RoundNum);

	UFUNCTION(BlueprintCallable)
		void UpdateScore(int Score);

	UFUNCTION(BlueprintCallable)
		void DisplayEndOfRoundText(ESlateVisibility textVisibility);
};