// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameModeBase.h"
#include "MenuPlayerController.h"
#include "MenuWidget.h"

#include "Blueprint/UserWidget.h"

AMenuGameModeBase::AMenuGameModeBase()
{
	PlayerControllerClass = AMenuPlayerController::StaticClass();
}

void AMenuGameModeBase::BeginPlay()
{
	Super::BeginPlay();

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