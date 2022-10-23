// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsWindow.generated.h"

class UTextBlock;
class USlider;
class UComboBoxString;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSettingsClosed);

UCLASS()
class TESTPROJECT_API USettingsWindow : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintCallable)
	FOnSettingsClosed OnSettingsClosed;

protected:
		
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSensitivityChanged(float Value);

	UFUNCTION()
	void OnVolumeChanged(float Value);
	
	UFUNCTION()
	void CloseSettings();
	
	UFUNCTION(BlueprintImplementableEvent)
	void FadeIn();

	UFUNCTION(BlueprintImplementableEvent)
	void FadeOut();
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	USlider* SensitivitySlider = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* SensitivityText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	USlider* VolumeSlider = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* VolumeText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class USoundButton* BackButton;
	
};
