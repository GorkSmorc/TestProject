// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

protected:

	UFUNCTION(BlueprintCallable)
	void UpdateHealth(int32 CurrentHealth, int32 MaxHealth);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components, meta=(BindWidget))
	class UProgressBar* HealthBar = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components, meta=(BindWidget))
	class UTextBlock* HealthText = nullptr;
	
};
