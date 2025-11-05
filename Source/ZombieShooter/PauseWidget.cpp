// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "MenuWidget.h"
#include "OptionsWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton->OnClicked.AddDynamic(this, &UPauseWidget::OnResumeClick);
	OptionsButton->OnClicked.AddDynamic(this, &UPauseWidget::OnOptionsClick);
	ExitButton->OnClicked.AddDynamic(this, &UPauseWidget::OnExitClick);
}

void UPauseWidget::OnResumeClick()
{
	RemoveFromParent();
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	APlayerController* PlayerController = Cast< APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly::FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}	
}

void UPauseWidget::OnOptionsClick()
{
	if (IsValid(OptionsWidget))
	{
		OptionsWidget->AddToViewport();
	}
	else
	{
		//Add Widget to viewport and remove current widget.
		if (IsValid(OptionsWidgetClass))
		{
			OptionsWidget = Cast<UOptionsWidget>(CreateWidget(GetWorld(), OptionsWidgetClass));

			if (OptionsWidget != nullptr)
			{
				OptionsWidget->AddToViewport();
			}
		}
	}	
}

void UPauseWidget::OnExitClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MenuMap");
}
