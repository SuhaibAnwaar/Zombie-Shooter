// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnResumeClick();

	UFUNCTION()
		void OnOptionsClick();

	UFUNCTION()
		void OnExitClick();

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ResumeText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* OptionsText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ExitText;

	UPROPERTY(meta = (BindWidget))
		class UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* OptionsButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ExitButton;

	//Reference to Options widget class.
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UUserWidget> OptionsWidgetClass;

	//The Options Widget that will be used.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		class UOptionsWidget* OptionsWidget;
};
