// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FinishGameScreen.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TESTPROJECT_API UFinishGameScreen : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void Init(bool bIsWin);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(EditDefaultsOnly)
	class UDataTable* Levels = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* MainText = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* KilledEnemyText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* ActionButtonText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton* ActionButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton* ExitButton = nullptr;

private:

	UFUNCTION()
	void OpenNextLevel();

	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void OpenMainMenu();
	
};
