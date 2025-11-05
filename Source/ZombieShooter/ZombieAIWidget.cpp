// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAIWidget.h"

#include "Components/ProgressBar.h"

void UZombieAIWidget::UpdateHealth(float DefaultHealth, float Health)
{
	HealthBar->SetPercent(Health / DefaultHealth);
}

