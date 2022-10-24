// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sound/AmbientSound.h"
#include "TestProjectAmbientSound.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API ATestProjectAmbientSound : public AAmbientSound
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, Category=Sound)
	TSoftObjectPtr<USoundBase> LoseSound;

	UPROPERTY(EditAnywhere, Category=Sound)
	TSoftObjectPtr<USoundBase> WinSound;
	
private:
	
	UFUNCTION()
	void OnVolumeChanged(float NewVolume);
		
	UFUNCTION()
	void SwitchSound(bool bIsWin);
	
	UFUNCTION()
	void ReturnSound();

	UPROPERTY()
	USoundBase* AmbientSound = nullptr;
	
	
};

