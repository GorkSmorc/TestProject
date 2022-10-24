// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/MainMenu.h"

#include "SettingsWindow.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/ProgressSave.h"
#include "Utilites/LevelsListDataTableStruct.h"
#include "Utilites/SoundButton.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GetOwningPlayer()->SetShowMouseCursor(true);

	if(PlayButton)
		PlayButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnPlay);

	if(SettingsButton)
		SettingsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OpenSettings);

	if(ExitButton)
		ExitButton->OnClicked.AddUniqueDynamic(this, &ThisClass::ExitGame);
}

void UMainMenu::OnPlay()
{
	if(!Levels)
	{
		return;
	}

	GetOwningPlayer()->SetInputMode(FInputModeGameOnly());

	TArray<FLevelsListDataTableStruct*> OutRows;
	Levels->GetAllRows<FLevelsListDataTableStruct>("",OutRows);

	const UProgressSave* Save = Cast<UProgressSave>(UGameplayStatics::LoadGameFromSlot("Progress",1));
	if(!Save)
	{
		if(OutRows[0])
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this,OutRows[0]->Level);
			return;
		}
	}
	const FString LastWonLevel = Save->GetLastWonLevel();
	for(size_t i = 0; i < OutRows.Num(); i++)
	{
		if(OutRows[i])
		{
			if(OutRows[i]->Level.GetAssetName() == LastWonLevel)
			{
				if(OutRows.IsValidIndex(i+1))
				{
					UGameplayStatics::OpenLevelBySoftObjectPtr(this, OutRows[i+1]->Level);
					return;
				}
				else
				{
					UGameplayStatics::OpenLevelBySoftObjectPtr(this, OutRows[i]->Level);
					return;
				}
			}
		}
	}
	
	if(OutRows[0])
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this,OutRows[0]->Level);
	}
}

void UMainMenu::OpenSettings()
{
	if(SettingsWindowClass)
	{
		FadeOutAnim();
		USettingsWindow* SettingsWindow = CreateWidget<USettingsWindow>(this,SettingsWindowClass);
		SettingsWindow->AddToViewport();
		SettingsWindow->OnSettingsClosed.AddUniqueDynamic(this, &ThisClass::FadeInAnim);
	}
}

void UMainMenu::ExitGame()
{
	FGenericPlatformMisc::RequestExit(false);
}