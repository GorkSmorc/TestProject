// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BotLeftsWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UBotLeftsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnSomeBotKilled(int32 BotsLeft) const;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* BotsLeftText = nullptr;
	
};
