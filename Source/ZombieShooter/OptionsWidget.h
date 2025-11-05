// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UOptionsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBackClick();

	UPROPERTY(meta = (BindWidget))
		class UImage* BackgroundImage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* BackText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MovementText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* JumpText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ShootText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ReloadText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ScrollText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MultiAmmoText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PhaseAmmoText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SprayAmmoText;

	UPROPERTY(meta = (BindWidget))
		class UButton* BackButton;

	//Reference to the Menu widget class.
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UUserWidget> WidgetClass;

	//The Menu Wdiget that will be used.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		class UMenuWidget* MenuWidget;
};
