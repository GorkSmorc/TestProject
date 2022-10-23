// Fill out your copyright notice in the Description page of Project Settings.


#include "Cheats/TestProjectCheatManager.h"

#include "CheatWidget.h"

void UTestProjectCheatManager::EnableCheats()
{
	if(!CheatWidget)
	{
		CheatWidget = CreateWidget<UCheatWidget>(GetWorld()->GetFirstPlayerController(),CheatWidgetClass);
	}

	if(!CheatWidget->IsInViewport())
	{
		CheatWidget->AddToViewport(20);
	}
}

void UTestProjectCheatManager::DisableCheats()
{
	if(CheatWidget)
	{
		if(CheatWidget->IsInViewport())
		{
			CheatWidget->RemoveFromParent();
		}
	}
}
