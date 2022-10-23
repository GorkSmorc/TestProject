// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BotLeftsWidget.h"

#include "Components/TextBlock.h"
#include "MainClasses/TestProjectGameMode.h"

void UBotLeftsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(ATestProjectGameMode* GameMode = GetWorld()->GetAuthGameMode<ATestProjectGameMode>())
	{
		GameMode->OnBotCountChanged.AddUObject(this, &ThisClass::OnSomeBotKilled);
		OnSomeBotKilled(GameMode->GetBotsLeft());
	}
}

void UBotLeftsWidget::OnSomeBotKilled(int32 BotsLeft) const
{
	if(BotsLeftText)
	{
		BotsLeftText->SetText(FText::AsNumber(BotsLeft));
	}
}
