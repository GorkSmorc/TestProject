// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthBar.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MainClasses/TestProjectPlayerState.h"

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(ATestProjectPlayerState* CastedPlayerState = Cast<ATestProjectPlayerState>(GetOwningPlayerState()))
	{
		CastedPlayerState->OnHealthChanged.AddUObject(this, &UHealthBar::UpdateHealth);
		UpdateHealth(CastedPlayerState->GetHealth(), CastedPlayerState->GetMaxHealth());
	}
}

void UHealthBar::UpdateHealth(int32 CurrentHealth, int32 MaxHealth)
{
	HealthText->SetText(FText::AsNumber(CurrentHealth));
	if(CurrentHealth > MaxHealth)
	{
		HealthText->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
	}
	else
	{
		HealthText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}
	
	
	HealthBar->SetPercent(static_cast<float>(CurrentHealth) / MaxHealth);
}
