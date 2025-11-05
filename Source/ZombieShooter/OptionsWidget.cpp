// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsWidget.h"
#include "MenuWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void UOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnClicked.AddDynamic(this, &UOptionsWidget::OnBackClick);
}

void UOptionsWidget::OnBackClick()
{
	RemoveFromParent();

	//If the game is not paused then the button was pressed in the menu level.
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		//Add Widget to viewport and remove current widget.
		if (IsValid(WidgetClass))
		{
			MenuWidget = Cast<UMenuWidget>(CreateWidget(GetWorld(), WidgetClass));

			if (MenuWidget != nullptr)
			{
				MenuWidget->AddToViewport();
			}
		}
	}
}

