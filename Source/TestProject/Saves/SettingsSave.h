// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SettingsSave.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API USettingsSave : public USaveGame
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void SetMouseSensitivity(float Sensitivity) { MouseSensitivity = Sensitivity; }

	UFUNCTION(BlueprintCallable)
	float GetMouseSensitivity() const { return MouseSensitivity; }
	
	UFUNCTION(BlueprintCallable)
	void SetSoundVolume(float Volume) { SoundVolume = Volume; }

	UFUNCTION(BlueprintCallable)
	float GetSoundVolume() const { return SoundVolume; }
	
private:

	UPROPERTY()
	float MouseSensitivity = 1.f;

	UPROPERTY()
	float SoundVolume = 1.f;
};
