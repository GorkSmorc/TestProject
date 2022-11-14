// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/SettingsWindow.h"

#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "MainClasses/TestProjectGameInstance.h"
#include "Utilities/SoundButton.h"

void USettingsWindow::NativeConstruct()
{
	Super::NativeConstruct();

	if(const UTestProjectGameInstance* GameInstance = GetGameInstance<UTestProjectGameInstance>())
	{
		if(SensitivitySlider)
		{
			SensitivitySlider->SetValue(GameInstance->GetMouseSensitivity());
			SensitivitySlider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::OnSensitivityChanged);
		}

		if(SensitivityText)
		{
			OnSensitivityChanged(GameInstance->GetMouseSensitivity());
		}

		if(VolumeSlider)
		{
			VolumeSlider->SetValue(GameInstance->GetSoundVolume());
			VolumeSlider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::OnVolumeChanged);
		}

		if(VolumeText)
		{
			OnVolumeChanged(GameInstance->GetSoundVolume());
		}
	}

	if(BackButton)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::CloseSettings);
	}
}

void USettingsWindow::OnSensitivityChanged(float Value)
{
	if(UTestProjectGameInstance* GameInstance = GetGameInstance<UTestProjectGameInstance>())
	{		
		FNumberFormattingOptions NumberFormat;				
		NumberFormat.MinimumIntegralDigits = 1;
		NumberFormat.MaximumIntegralDigits = 1;
		NumberFormat.MinimumFractionalDigits = 2;
		NumberFormat.MaximumFractionalDigits = 2;
		
		SensitivityText->SetText(FText::AsNumber(Value,&NumberFormat));
		GameInstance->SetMouseSensitivity(Value);
	}
}

void USettingsWindow::OnVolumeChanged(float Value)
{
	if(UTestProjectGameInstance* GameInstance = GetGameInstance<UTestProjectGameInstance>())
	{		
		FNumberFormattingOptions NumberFormat;				
		NumberFormat.MinimumIntegralDigits = 1;
		NumberFormat.MaximumIntegralDigits = 1;
		NumberFormat.MinimumFractionalDigits = 2;
		NumberFormat.MaximumFractionalDigits = 2;
		
		VolumeText->SetText(FText::AsNumber(Value, &NumberFormat));
		GameInstance->SetSoundVolume(Value);
	}
}

void USettingsWindow::CloseSettings()
{
	FadeOut();
	OnSettingsClosed.Broadcast();

	if(UTestProjectGameInstance* GameInstance = GetGameInstance<UTestProjectGameInstance>())
	{
		GameInstance->SaveSettings();
	}
}
