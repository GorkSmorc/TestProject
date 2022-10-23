// Fill out your copyright notice in the Description page of Project Settings.


#include "MainClasses/TestProjectGameInstance.h"

#include "Cheats/TestProjectCheatManager.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/SettingsSave.h"

void UTestProjectGameInstance::Init()
{
	Super::Init();
	
	LoadSettings();
}

void UTestProjectGameInstance::ShowCheats()
{
	if(GetWorld() && GetWorld()->GetFirstPlayerController())
	{
		if(UTestProjectCheatManager* CheatManager = Cast<UTestProjectCheatManager>(GetWorld()->GetFirstPlayerController()->CheatManager))
		{
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
			CheatManager->EnableCheats();
		}
	}
}

void UTestProjectGameInstance::HideCheats()
{
	if(GetWorld() && GetWorld()->GetFirstPlayerController())
	{
		if(UTestProjectCheatManager* CheatManager = Cast<UTestProjectCheatManager>(GetWorld()->GetFirstPlayerController()->CheatManager))
		{
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
			GetWorld()->GetFirstPlayerController()->SetInputMode((FInputModeGameOnly()));
			CheatManager->DisableCheats();
		}
	}
}

void UTestProjectGameInstance::SaveSettings()
{
	if(!SettingsSave)
	{
		SettingsSave = Cast<USettingsSave>(UGameplayStatics::CreateSaveGameObject(USettingsSave::StaticClass()));
		if(!SettingsSave)
		{
			return;
		}
	}

	SettingsSave->SetMouseSensitivity(MouseSensitivity);
	SettingsSave->SetSoundVolume(SoundsVolume);
	UGameplayStatics::SaveGameToSlot(SettingsSave,TEXT("Settings"),1);
	
}

void UTestProjectGameInstance::LoadSettings()
{
	SettingsSave = Cast<USettingsSave>(UGameplayStatics::LoadGameFromSlot(TEXT("Settings"),1));
	if(SettingsSave)
	{
		MouseSensitivity = SettingsSave->GetMouseSensitivity();
		SoundsVolume = SettingsSave->GetSoundVolume();
	}
}

void UTestProjectGameInstance::SetMouseSensitivity(float Sensitivity)
{
	MouseSensitivity = Sensitivity;
}

void UTestProjectGameInstance::SetSoundVolume(float Volume)
{
	SoundsVolume = Volume;
	OnSoundVolumeChanged.Broadcast(Volume);
}
