// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "OptionsWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->OnClicked.AddDynamic(this, &UMenuWidget::OnPlayClick);
	OptionsButton->OnClicked.AddDynamic(this, &UMenuWidget::OnOptionsClick);
	ExitButton->OnClicked.AddDynamic(this, &UMenuWidget::OnExitClick);
}

void UMenuWidget::OnPlayClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), "FirstPersonExampleMap");
}

void UMenuWidget::OnOptionsClick()
{
	//Add Widget to viewport and remove current widget.
	if (IsValid(WidgetClass))
	{
		RemoveFromParent();
		OptionsWidget = Cast<UOptionsWidget>(CreateWidget(GetWorld(), WidgetClass));

		if (OptionsWidget != nullptr)
		{
			OptionsWidget->AddToViewport();
		}
	}
}

void UMenuWidget::OnExitClick()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, false);
}
