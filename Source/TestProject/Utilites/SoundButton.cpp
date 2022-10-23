// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilites/SoundButton.h"

#include "Kismet/GameplayStatics.h"
#include "MainClasses/TestProjectGameInstance.h"
#include "UObject/ConstructorHelpers.h"

USoundButton::USoundButton() : Super()
{
	const ConstructorHelpers::FObjectFinder<USoundWave> Sound = ConstructorHelpers::FObjectFinder<USoundWave>(TEXT("/GAME/FirstPersonCPP/Audio/Button.Button"));
	if(Sound.Succeeded())
	{
		ClickSound = Sound.Object;
	}
}

void USoundButton::PostLoad()
{
	Super::PostLoad();
	OnClicked.AddUniqueDynamic(this, &ThisClass::PlayClickSound);
}

void USoundButton::PlayClickSound()
{
	if(ClickSound)
	{
		UGameplayStatics::PlaySound2D(this,ClickSound,GetGameInstance<UTestProjectGameInstance>()->GetSoundVolume());
	}
}
