// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnPlayClick();

	UFUNCTION()
	void OnOptionsClick();

	UFUNCTION()
	void OnExitClick();

	UPROPERTY(meta = (BindWidget))
		class UImage* BackgroundImage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TitleText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* OptionsText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ExitText;

	UPROPERTY(meta = (BindWidget))
		class UButton* PlayButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* OptionsButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ExitButton;

	//Reference to Options widget class.
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UUserWidget> WidgetClass;

	//The Options Widget that will be used.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		class UOptionsWidget* OptionsWidget;	
};
