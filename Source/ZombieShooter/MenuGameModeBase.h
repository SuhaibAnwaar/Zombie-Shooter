// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMenuGameModeBase();

	void BeginPlay() override;

	//Reference to the Menu widget class.
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UUserWidget> WidgetClass;

	//The Menu Wdiget that will be used.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		class UMenuWidget* MenuWidget;
};
