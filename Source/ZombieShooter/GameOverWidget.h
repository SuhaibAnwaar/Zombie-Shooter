// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnBackClick();

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* BackText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* GameOverText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ScoreText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ActualScoreText;

	UPROPERTY(meta = (BindWidget))
		class UButton* BackButton;
};
