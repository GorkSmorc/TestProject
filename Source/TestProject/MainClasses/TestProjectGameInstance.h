// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TestProjectGameInstance.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoundVolumeChanged, float, NewVolume);

UCLASS()
class TESTPROJECT_API UTestProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;
	
public:

	UFUNCTION(BlueprintCallable,Exec)
	void ShowCheats();

	UFUNCTION(BlueprintCallable,Exec)
	void HideCheats();

	UFUNCTION(BlueprintCallable, Category="Settings")
	void SaveSettings();

	UFUNCTION(BlueprintCallable, Category="Settings")
	void LoadSettings();

	UFUNCTION(BlueprintCallable, Category="Settings")
	void SetMouseSensitivity(float Sensitivity);

	UFUNCTION(BlueprintCallable, Category="Settings")
	float GetMouseSensitivity() const { return MouseSensitivity; }
	
	UFUNCTION(BlueprintCallable, Category="Settings")
	void SetSoundVolume(float Volume);

	UFUNCTION(BlueprintCallable, Category="Settings")
	float GetSoundVolume() const { return SoundsVolume; }

	UPROPERTY(BlueprintAssignable)
	FOnSoundVolumeChanged OnSoundVolumeChanged;


private:
	
	UPROPERTY()
	float MouseSensitivity = 1.f;

	UPROPERTY()
	float SoundsVolume = 1.f;

	UPROPERTY()
	class USettingsSave* SettingsSave = nullptr;
		
	
};
