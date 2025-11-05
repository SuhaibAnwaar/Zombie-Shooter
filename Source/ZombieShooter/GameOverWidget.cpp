// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "ZombieShooterCharacter.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnBackClick);

	AZombieShooterCharacter* PlayerRef = Cast<AZombieShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerRef)
	{
		ActualScoreText->SetText(FText::FromString(FString::FromInt(PlayerRef->GetScore())));
	}
}

void UGameOverWidget::OnBackClick()
{
	RemoveFromParent();
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	APlayerController* PlayerController = Cast< APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly::FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}

	UGameplayStatics::OpenLevel(GetWorld(), "MenuMap");
}