// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "SoundButton.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API USoundButton : public UButton
{
	GENERATED_BODY()

	USoundButton();

public:

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	USoundBase* ClickSound = nullptr;

protected:
	
	virtual void PostLoad() override;

	UFUNCTION()
	void PlayClickSound();
};
