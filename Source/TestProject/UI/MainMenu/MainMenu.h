// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class USoundButton;
/**
 * 
 */
UCLASS()
class TESTPROJECT_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnPlay();
	
	UFUNCTION()
	void OpenSettings();
	
	UFUNCTION()
	void ExitGame();

	UFUNCTION(BlueprintImplementableEvent)
	void FadeInAnim();
	
	UFUNCTION(BlueprintImplementableEvent)
	void FadeOutAnim();

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* Levels = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	USoundButton* PlayButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	USoundButton* SettingsButton = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	USoundButton* ExitButton = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USettingsWindow> SettingsWindowClass = nullptr;
};
