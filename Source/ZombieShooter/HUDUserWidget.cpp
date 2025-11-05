// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDUserWidget.h"
#include "ZombieShooterCharacter.h"
#include "WeaponActor.h"

#include "Kismet\GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Materials/Material.h"

void UHUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerRef = Cast<AZombieShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	RechargeBar->BarFillType = EProgressBarFillType::BottomToTop;
	RechargeBar->RenderOpacity = 0.2f;
	RoundText->SetText(FText::FromString("Round"));
	EndOfRoundText->SetVisibility(ESlateVisibility::Hidden);
}

void UHUDUserWidget::UpdateHealth(float DefaultHealth, float Health)
{
	HealthBar->SetPercent(Health / DefaultHealth);
}

void UHUDUserWidget::UpdateAmmo()
{	
	//Convert int value to text.
	Ammo->SetText(FText::FromString(FString::FromInt(PlayerRef->Weapon->CurrentAmmo)));
	Clip->SetText(FText::FromString(FString::FromInt(PlayerRef->Weapon->CurrentClip)));
}

void UHUDUserWidget::UpdateRechargeBar(float Recharge, float MaxRecharge)
{
	RechargeBar->SetPercent(Recharge / MaxRecharge);
}

void UHUDUserWidget::UpdateAmmoImages()
{
	if (AmmoTex[0] && AmmoTex[1] && AmmoTex[2] && AmmoTex[3] && AmmoTex[4] && NoAmmoMat)
	{
		switch (PlayerRef->Weapon->GetCurrentAmmoType())
		{
		case EAmmoType::MultiAmmo:
			switch (PlayerRef->Weapon->GetCurrentMultiAmmoType())
			{
			case EMultiAmmoType::Original:
				CurrentAmmo->SetBrushFromTexture(AmmoTex[0]);
				Ammo2->SetBrushFromTexture(AmmoTex[1]);
				Ammo3->SetBrushFromTexture(AmmoTex[2]);
				break;
			case EMultiAmmoType::Ice:
				CurrentAmmo->SetBrushFromTexture(AmmoTex[1]);
				Ammo2->SetBrushFromTexture(AmmoTex[2]);
				Ammo3->SetBrushFromTexture(AmmoTex[0]);
				break;
			case EMultiAmmoType::Fire:
				CurrentAmmo->SetBrushFromTexture(AmmoTex[2]);
				Ammo2->SetBrushFromTexture(AmmoTex[0]);
				Ammo3->SetBrushFromTexture(AmmoTex[1]);
			}
			break;
		case EAmmoType::PhaseAmmo:	//when the phase ammo is being used then the HUD should only show the phase ammo texture and not any other ammo textures.
			CurrentAmmo->SetBrushFromTexture(AmmoTex[3]);
			Ammo2->SetBrushFromMaterial(NoAmmoMat);
			Ammo3->SetBrushFromMaterial(NoAmmoMat);
			break;
		case EAmmoType::SprayAmmo:	//when the spray ammo is being used then the HUD should only show the spray ammo texture and not any other ammo textures.
			CurrentAmmo->SetBrushFromTexture(AmmoTex[4]);
			Ammo2->SetBrushFromMaterial(NoAmmoMat);
			Ammo3->SetBrushFromMaterial(NoAmmoMat);
		}
	}

}

void UHUDUserWidget::UpdateRoundNumber(int RoundNum)
{
	Round->SetText(FText::FromString(FString::FromInt(RoundNum)));
}

void UHUDUserWidget::UpdateScore(int Score)
{
	ActualScoreText->SetText(FText::FromString(FString::FromInt(Score)));
}

void UHUDUserWidget::DisplayEndOfRoundText(ESlateVisibility textVisibility)
{
	EndOfRoundText->SetVisibility(textVisibility);
}
