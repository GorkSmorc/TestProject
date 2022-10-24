// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilites/TestProjectAmbientSound.h"

#include "Components/AudioComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "MainClasses/TestProjectGameInstance.h"
#include "MainClasses/TestProjectGameMode.h"

void ATestProjectAmbientSound::BeginPlay()
{
	Super::BeginPlay();

	AmbientSound = GetAudioComponent()->Sound;

	if(UTestProjectGameInstance* GameInstance = GetGameInstance<UTestProjectGameInstance>())
	{
		GetAudioComponent()->SetVolumeMultiplier(GameInstance->GetSoundVolume());
		GameInstance->OnSoundVolumeChanged.AddUniqueDynamic(this, &ThisClass::OnVolumeChanged);
	}

	if(ATestProjectGameMode* GameMode = GetWorld()->GetAuthGameMode<ATestProjectGameMode>())
	{
		GameMode->OnGameEnded.AddUObject(this, &ThisClass::SwitchSound);
		GameMode->OnGameRestarted.AddUniqueDynamic(this, &ThisClass::ReturnSound);
	}

	GetAudioComponent()->bIsUISound = true;
}

void ATestProjectAmbientSound::OnVolumeChanged(float NewVolume)
{
	GetAudioComponent()->SetVolumeMultiplier(NewVolume);
}

void ATestProjectAmbientSound::SwitchSound(bool bIsWin)
{
	GetAudioComponent()->FadeOut(1.f,0.f);
	
	TSoftObjectPtr<USoundBase> SoundToLoad = bIsWin ? WinSound : LoseSound;
	
	FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
	StreamableManager.RequestAsyncLoad(SoundToLoad.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this,[this,SoundToLoad]() 
	{
		if(USoundBase* Sound = SoundToLoad.Get())
		{
			GetAudioComponent()->Sound = Sound;
			GetAudioComponent()->FadeIn(1.f);
		}
	}));
	
}

void ATestProjectAmbientSound::ReturnSound()
{
	GetAudioComponent()->Sound = AmbientSound;
	GetAudioComponent()->FadeIn(1.f);
}
