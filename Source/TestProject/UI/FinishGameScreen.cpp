// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FinishGameScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "MainClasses/TestProjectGameMode.h"
#include "Utilites/LevelsListDataTableStruct.h"

void UFinishGameScreen::Init(bool bIsWin)
{
	if(MainText)
		MainText->SetText(FText::FromString(bIsWin ? TEXT("You Win!") : TEXT("Game over")));

	if(ActionButtonText)
		ActionButtonText->SetText(FText::FromString(bIsWin ? TEXT("Continiue") : TEXT("Restart")));

	if(ActionButton)
	{
		if(bIsWin)
			ActionButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OpenNextLevel);
		else
			ActionButton->OnClicked.AddUniqueDynamic(this, &ThisClass::RestartGame);
	}

	if(ExitButton)
		ExitButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OpenMainMenu);

	if(const ATestProjectGameMode* GameMode = GetWorld()->GetAuthGameMode<ATestProjectGameMode>())
	{
		KilledEnemyText->SetText(FText::AsNumber(GameMode->GetKilledBotsCount()));
	}
}

void UFinishGameScreen::NativeConstruct()
{
	Super::NativeConstruct();

	GetOwningPlayer()->SetInputMode(FInputModeUIOnly());
	GetOwningPlayer()->FlushPressedKeys();
}

void UFinishGameScreen::NativeDestruct()
{
	if(GetOwningPlayer())
	{
		GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
	}

	Super::NativeDestruct();
}

void UFinishGameScreen::OpenNextLevel()
{
	if(Levels)
	{
		TArray<FLevelsListDataTableStruct*> OutRows;
		Levels->GetAllRows<FLevelsListDataTableStruct>("",OutRows);
		for(size_t i = 0; i < OutRows.Num(); i++)
		{
			if(OutRows[i] && OutRows[i]->Level)
			{
				if(OutRows[i]->Level->GetName() == GetWorld()->GetName())
				{
					if(OutRows.IsValidIndex(i+1))
					{
						UGameplayStatics::OpenLevelBySoftObjectPtr(this,OutRows[i+1]->Level);
						break;
					}
				}

				UGameplayStatics::OpenLevel(this,TEXT("MainMenu"));
				break;
			}
		}
	}
}

void UFinishGameScreen::RestartGame()
{
	if(ATestProjectGameMode* GameMode = GetWorld()->GetAuthGameMode<ATestProjectGameMode>())
	{
		GetOwningPlayer()->SetShowMouseCursor(false);
		GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
		RemoveFromParent();
		
		GameMode->ResetGame();
	}
}

void UFinishGameScreen::OpenMainMenu()
{
	UGameplayStatics::OpenLevel(this,TEXT("MainMenu"));
}
